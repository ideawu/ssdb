#include "util/fde.h"
#include "slave.h"
#include "t_kv.h"
#include "t_hash.h"
#include "t_zset.h"
#include "include.h"

Slave::Slave(SSDB *ssdb, leveldb::DB* meta_db, const char *ip, int port, bool is_mirror){
	thread_quit = false;
	this->ssdb = ssdb;
	this->meta_db = meta_db;
	this->master_ip = std::string(ip);
	this->master_port = port;
	this->is_mirror = is_mirror;
	if(this->is_mirror){
		this->log_type = BinlogType::MIRROR;
	}else{
		this->log_type = BinlogType::SYNC;
	}
	
	this->link = NULL;
	this->last_seq = 0;
	this->last_key = "";
	this->connect_retry = 0;

	load_status();
	log_debug("last_seq: %llu, last_key: %s",
		last_seq, hexmem(last_key.data(), last_key.size()).c_str());
}

Slave::~Slave(){
	log_debug("stopping slave thread...");
	if(!thread_quit){
		stop();
	}
	if(link){
		delete link;
	}
	log_debug("Slave finalized");
}

void Slave::start(){
	thread_quit = false;
	int err = pthread_create(&run_thread_tid, NULL, &Slave::_run_thread, this);
	if(err != 0){
		log_error("can't create thread: %s", strerror(err));
	}
}

void Slave::stop(){
	thread_quit = true;
	void *tret;
	int err = pthread_join(run_thread_tid, &tret);
    if(err != 0){
		log_error("can't join thread: %s", strerror(err));
	}
}

std::string Slave::status_key(){
	static std::string key;
	if(key.empty()){
		char buf[100];
		snprintf(buf, sizeof(buf), "new.slave.status|%s|%d", master_ip.c_str(), master_port);
		key.assign(buf);
	}
	return key;
}

void Slave::load_status(){
	std::string key = status_key();
	std::string val;
	leveldb::Status s = meta_db->Get(leveldb::ReadOptions(), key, &val);
	if(s.ok()){
		if(val.size() < sizeof(uint64_t)){
			log_error("invalid format of status");
		}else{
			last_seq = *((uint64_t *)(val.data()));
			last_key.assign(val.data() + sizeof(uint64_t), val.size() - sizeof(uint64_t));
		}
	}
}

void Slave::save_status(){
	std::string key = status_key();
	std::string val;
	val.append((char *)&this->last_seq, sizeof(uint64_t));
	val.append(this->last_key);
	meta_db->Put(leveldb::WriteOptions(), key, val);
}

int Slave::connect(){
	const char *ip = this->master_ip.c_str();
	int port = this->master_port;
	
	if(++connect_retry > 101){
		connect_retry = 62;
	}
	//log_debug("%d", retry);
	// TODO: 找一个公式
	if(connect_retry == 1 || connect_retry == 31 || connect_retry == 61 || connect_retry == 101){
		log_info("[%d] connecting to master at %s:%d...", connect_retry-1, ip, port);
		link = Link::connect(ip, port);
		if(link == NULL){
			log_error("failed to connect to master: %s:%d!", ip, port);
			goto err;
		}else{
			connect_retry = 0;
			char seq_buf[20];
			sprintf(seq_buf, "%llu", this->last_seq);
			
			const char *type = is_mirror? "mirror" : "sync";
			
			link->send("sync140", seq_buf, this->last_key, type);
			if(link->flush() == -1){
				log_error("network error");
				delete link;
				link = NULL;
				goto err;
			}
			log_info("ready to receive binlogs...");
			return 1;
		}
	}
	return 0;
err:
	return -1;
}

void* Slave::_run_thread(void *arg){
	Slave *slave = (Slave *)arg;
	const std::vector<Bytes> *req;
	Fdevents select;
	const Fdevents::events_t *events;
	int idle = 0;
	bool reconnect = false;
	
#define RECV_TIMEOUT	200
#define MAX_RECV_IDLE	30 * 1000/RECV_TIMEOUT

	while(!slave->thread_quit){
		if(reconnect){
			reconnect = false;
			select.del(slave->link->fd());
			delete slave->link;
			slave->link = NULL;
		}
		if(!slave->connected()){
			if(slave->connect() != 1){
				usleep(100 * 1000);
			}else{
				select.set(slave->link->fd(), FDEVENT_IN, 0, NULL);
			}
			continue;
		}
		
		events = select.wait(RECV_TIMEOUT);
		if(events == NULL){
			log_fatal("events.wait error: %s", strerror(errno));
			break;
		}else if(events->empty()){
			if(idle++ >= MAX_RECV_IDLE){
				log_error("the master haven't responsed for awhile, reconnect...");
				idle = 0;
				reconnect = true;
			}
			continue;
		}
		idle = 0;

		if(slave->link->read() <= 0){
			log_error("link.read error, reconnecting to master...");
			reconnect = true;
			continue;
		}

		while(1){
			req = slave->link->recv();
			if(req == NULL){
				log_error("link.recv error, reconnecting to master...");
				reconnect = true;
				break;
			}else if(req->empty()){
				break;
			}else{
				if(slave->proc(*req) == -1){
					goto quit_;
				}
			}
		}
	} // end while

quit_:
	// TODO: must notify the main thread to quit
	log_info("Slave thread quit");
	return (void *)NULL;
}

int Slave::proc(const std::vector<Bytes> &req){
	Binlog log;
	if(log.load(req[0].Slice()) == -1){
		log_error("invalid binlog!");
		return 0;
	}
	if(log.type() != BinlogType::NOOP){
		if(this->is_mirror){
			log_debug("[mirror] %s", log.dumps().c_str());
		}else{
			log_debug("[sync] %s", log.dumps().c_str());
		}
	}
	switch(log.type()){
		case BinlogType::NOOP:
			this->proc_noop(log, req);
			break;
		case BinlogType::COPY:
			this->proc_copy(log, req);
			break;
		case BinlogType::SYNC:
		case BinlogType::MIRROR:
			this->proc_sync(log, req);
			break;
		default:
			break;
	}
	return 0;
}

int Slave::proc_noop(const Binlog &log, const std::vector<Bytes> &req){
	uint64_t seq = log.seq();
	if(this->last_seq != seq){
		log_debug("noop last_seq: %llu, seq: %llu", this->last_seq, seq);
		this->last_seq = seq;
		this->save_status();
	}
	return 0;
}

int Slave::proc_copy(const Binlog &log, const std::vector<Bytes> &req){
	switch(log.cmd()){
		case BinlogCommand::BEGIN:
			log_info("copy begin");
			break;
		case BinlogCommand::END:
			log_info("copy end, step in sync");
			this->last_key = "";
			this->save_status();
			break;
		default:
			proc_sync(log, req);
			break;
	}
	return 0;
}

int Slave::proc_sync(const Binlog &log, const std::vector<Bytes> &req){
	switch(log.cmd()){
		case BinlogCommand::KSET:
			{
				if(req.size() != 2){
					break;
				}
				std::string key;
				if(decode_kv_key(log.key(), &key) == -1){
					break;
				}
				Bytes val = req[1];
				log_trace("set %s", hexmem(key.data(), key.size()).c_str());
				ssdb->set(key, req[1], log_type);
			}
			break;
		case BinlogCommand::KDEL:
			{
				std::string key;
				if(decode_kv_key(log.key(), &key) == -1){
					break;
				}
				log_trace("del %s", hexmem(key.data(), key.size()).c_str());
				ssdb->del(key, log_type);
			}
			break;
		case BinlogCommand::HSET:
			{
				if(req.size() != 2){
					break;
				}
				std::string name, key;
				if(decode_hash_key(log.key(), &name, &key) == -1){
					break;
				}
				Bytes val = req[1];
				log_trace("hset %s %s",
					hexmem(name.data(), name.size()).c_str(),
					hexmem(key.data(), key.size()).c_str());
				ssdb->hset(name, key, req[1], log_type);
			}
			break;
		case BinlogCommand::HDEL:
			{
				std::string name, key;
				if(decode_hash_key(log.key(), &name, &key) == -1){
					break;
				}
				log_trace("hdel %s %s",
					hexmem(name.data(), name.size()).c_str(),
					hexmem(key.data(), key.size()).c_str());
				ssdb->hdel(name, key, log_type);
			}
			break;
		case BinlogCommand::ZSET:
			{
				if(req.size() != 2){
					break;
				}
				std::string name, key;
				if(decode_zset_key(log.key(), &name, &key) == -1){
					break;
				}
				Bytes val = req[1];
				log_trace("zset %s %s",
					hexmem(name.data(), name.size()).c_str(),
					hexmem(key.data(), key.size()).c_str());
				ssdb->zset(name, key, req[1], log_type);
			}
			break;
		case BinlogCommand::ZDEL:
			{
				std::string name, key;
				if(decode_zset_key(log.key(), &name, &key) == -1){
					break;
				}
				log_trace("zdel %s %s",
					hexmem(name.data(), name.size()).c_str(),
					hexmem(key.data(), key.size()).c_str());
				ssdb->zdel(name, key, log_type);
			}
			break;
		default:
			log_error("unknown binlog, type=%d, cmd=%d", log.type(), log.cmd());
			break;
	}
	this->last_seq = log.seq();
	if(log.type() == BinlogType::COPY){
		this->last_key = log.key().String();
	}
	this->save_status();
	return 0;
}

