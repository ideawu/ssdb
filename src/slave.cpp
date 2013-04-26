#include "util/fde.h"
#include "slave.h"
#include "t_kv.h"

Slave::Slave(const SSDB *ssdb, leveldb::DB* meta_db, const char *ip, int port, bool is_mirror){
	thread_quit = false;
	this->ssdb = ssdb;
	this->meta_db = meta_db;
	this->master_ip = std::string(ip);
	this->master_port = port;
	this->is_mirror = is_mirror;
	
	this->link = NULL;
	this->last_seq = 0;
	this->last_key = "";
	this->connect_retry = 0;

	load_status();
	log_debug("last_seq: %llu, last_key: %s",
		last_seq, hexmem(last_key.data(), last_key.size()).c_str());
	this->last_seq = 0;
	this->last_key = "";
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
		snprintf(buf, sizeof(buf), "slave.status|%s|%d", master_ip.c_str(), master_port);
		key.assign(buf);
	}
	return key;
}

void Slave::load_status(){
	std::string key = status_key();
	std::string status;
	leveldb::Status s;
	s = meta_db->Get(leveldb::ReadOptions(), key, &status);
	if(s.IsNotFound()){
		return;
	}
	if(s.ok()){
		if(status.size() < sizeof(uint64_t)){
			log_error("invlid format of status");
		}else{
			last_seq = *((uint64_t *)(status.data()));
			last_key = std::string(status.data() + sizeof(uint64_t), status.size() - sizeof(uint64_t));
			log_debug("load_status seq: %llu, key: %s",
				last_seq,
				hexmem(last_key.data(), last_key.size()).c_str());
		}
	}
}

void Slave::save_status(){
	std::string key = status_key();
	std::string status;
	status.append((char *)&this->last_seq, sizeof(uint64_t));
	status.append(this->last_key);
	meta_db->Put(leveldb::WriteOptions(), key, status);
}

template<class T>
static std::string serialize_req(T &req){
	std::string ret;
	char buf[50];
	for(int i=0; i<(int)req.size(); i++){
		if(i >= 5 && i < (int)req.size() - 1){
			sprintf(buf, "[%d more...]", (int)req.size() - i - 1);
			ret.append(buf);
			break;
		}
		if(((req[0] == "get" || req[0] == "set") && i == 1) || req[i].size() < 30){
			std::string h = hexmem(req[i].data(), req[i].size());
			ret.append(h);
		}else{
			sprintf(buf, "[%d bytes]", (int)req[i].size());
			ret.append(buf);
		}
		if(i < (int)req.size() - 1){
			ret.append(" ");
		}
	}
	return ret;
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
		log_info("[%d] connecting to master at %s:%d...", connect_retry, ip, port);
		link = Link::connect(ip, port);
		if(link == NULL){
			log_error("failed to connect to master: %s:%d!", ip, port);
			goto err;
		}else{
			connect_retry = 0;
			char seq_buf[20];
			sprintf(seq_buf, "%llu", this->last_seq);

			link->output->append_record("sync");
			link->output->append_record(seq_buf);
			link->output->append_record(this->last_key);
			if(is_mirror){
				link->output->append_record("mirror");
			}
			link->output->append('\n');

			if(link->flush() == -1){
				log_error("network error");
				delete link;
				link = NULL;
				goto err;
			}
			log_info("ready to receive sync commands...");
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
	log_debug("%s", log.dumps().c_str());
	switch(log.type()){
		case BinlogType::NOOP:
			this->proc_sync(log, req);
			break;
		case BinlogType::DUMP:
			this->proc_dump(log, req);
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

int Slave::proc_dump(const Binlog &log, const std::vector<Bytes> &req){
	switch(log.cmd()){
		case BinlogCommand::BEGIN:
			log_info("dump begin");
			break;
		case BinlogCommand::END:
			log_info("dump end, step in sync");
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
			if(req.size() != 2){
				//
			}else{
				std::string key;
				if(decode_kv_key(log.key(), &key) == -1){
					//
				}else{
					Bytes val = req[1];
					log_trace("set %s %s",
						hexmem(key.data(), key.size()).c_str(),
						hexmem(val.data(), val.size()).c_str());
					//ssdb->set(key, val);
					//this->save_status();
				}
			}
			break;
		case BinlogCommand::KDEL:
			break;
		case BinlogCommand::HSET:
			break;
		case BinlogCommand::HDEL:
			break;
		case BinlogCommand::ZSET:
			break;
		case BinlogCommand::ZDEL:
			break;
	}
	return 0;
}

