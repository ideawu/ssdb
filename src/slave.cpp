#include <pthread.h>
#include <vector>
#include "slave.h"

Slave::Slave(const SSDB *ssdb, leveldb::DB* meta_db, const char *ip, int port){
	this->ssdb = ssdb;
	this->meta_db = meta_db;
	this->master_ip = std::string(ip);
	this->master_port = port;

	this->last_seq = 0;
	this->last_key = "";

	load_status();
	log_debug("last_seq: %llu, last_key: %s",
		last_seq, hexmem(last_key.data(), last_key.size()).c_str());
}

Slave::~Slave(){
	log_debug("stopping slave thread...");
	stop();
	log_debug("Slave finalized");
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
	std::string key = "slave.status|";
	key.append(master_ip);
	key.append("|");
	char buf[10];
	sprintf(buf, "%d", master_port);
	key.append(buf);
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

void Slave::start(){
	thread_quit = false;
	int err = pthread_create(&run_thread_tid, NULL, &Slave::_run_thread, this);
	if(err != 0){
		log_error("can't create thread: %s", strerror(err));
	}
}

template<class T>
static std::string serialize_req(T &req){
	std::string ret;
	char buf[50];
	for(int i=0; i<req.size(); i++){
		if(i >= 5 && i < req.size() - 1){
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
		if(i < req.size() - 1){
			ret.append(" ");
		}
	}
	return ret;
}

static Link* connect(const char *ip, int port, uint64_t last_seq, std::string &last_key){
	Link *link = Link::connect(ip, port);
	if(link == NULL){
		log_error("failed to connect to master: %s:%d!", ip, port);
		goto err;
	}
	// TODO: set link.recv_timeout

	char seq_buf[20];
	sprintf(seq_buf, "%llu", last_seq);

	link->output->append_record("sync");
	link->output->append_record(seq_buf);
	link->output->append_record(last_key);
	link->output->append('\n');

	if(link->flush() == -1){
		log_error("network error");
		goto err;
	}

	log_info("ready to receive sync commands...");
	return link;
err:
	return NULL;
}

void* Slave::_run_thread(void *arg){
	Slave *slave = (Slave *)arg;
	const SSDB *ssdb = slave->ssdb;
	const char *ip = slave->master_ip.c_str();
	int port = slave->master_port;
	Link *link = NULL;

	int retry = 0;
	const std::vector<Bytes> *req;
	while(!slave->thread_quit){
		if(link == NULL){
			usleep(100 * 1000);
			if(retry > 100){
				retry = 61;
			}
			// TODO: 找一个公式
			if(retry == 0 || retry == 10 || retry == 30 || retry == 60 || retry == 100){
				log_info("[%d] connecting to master at %s:%d...", retry, ip, port);
				link = connect(ip, port, slave->last_seq, slave->last_key);
			}
			if(link == NULL){
				retry ++;
				continue;
			}else{
				retry = 0;
			}
		}

		req = link->recv();
		if(req == NULL){
			retry = 1;
			delete link;
			link = NULL;
			log_info("recv error, reconnecting to master...");
			continue;
		}else if(req->empty()){
			if(link->read() <= 0){
				retry = 1;
				delete link;
				link = NULL;
				log_info("network error, reconnecting to master...");
			}
			continue;
		}

		Bytes cmd = req->at(0);
		if(cmd == "dump_begin"){
			log_info("dump begin");
			// TODO: patch-diff old data
		}else if(cmd == "dump_end"){
			log_info("dump end, step in sync");
			slave->last_key = "";
			slave->save_status();
		}else if(cmd == "dump_set"){
			log_debug("%s", serialize_req(*req).c_str());
			if(req->size() != 4){
				log_warn("invalid dump_set params!");
				break;
			}
			uint64_t seq = req->at(1).Uint64();
			Bytes key = req->at(2);
			Bytes val = req->at(3);
			slave->last_key = key.String();
			if(seq > 0){
				slave->last_seq = seq;
			}

			int ret = ssdb->raw_set(key, val);
			if(ret == -1){
				log_error("ssdb.raw_set error!");
			}
			slave->save_status();
		}else if(cmd == "sync_set"){
			log_debug("%s", serialize_req(*req).c_str());
			if(req->size() != 4){
				log_warn("invalid sync_set params!");
				break;
			}
			uint64_t seq = req->at(1).Uint64();
			Bytes key = req->at(2);
			Bytes val = req->at(3);
			// sync
			slave->last_seq = seq;

			int ret = ssdb->raw_set(key, val);
			if(ret == -1){
				log_error("ssdb.raw_set error!");
			}
			slave->save_status();
		}else if(cmd == "sync_del"){
			log_debug("%s", serialize_req(*req).c_str());
			if(req->size() != 3){
				log_warn("invalid del params!");
				break;
			}
			uint64_t seq = req->at(1).Uint64();
			Bytes key = req->at(2);
			slave->last_seq = seq;

			int ret = ssdb->raw_del(key);
			if(ret == -1){
				log_error("ssdb.raw_del error!");
			}

			slave->save_status();
		}else if(cmd == "noop"){
			//
		}else{
			log_warn("unknow sync command: %s", serialize_req(*req).c_str());
		}
	} // end while

	if(link){
		delete link;
	}
	log_info("Slave thread quit");
	return (void *)NULL;
}
