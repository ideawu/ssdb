/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "net/fde.h"
#include "util/log.h"
#include "slave.h"
#include "include.h"

Slave::Slave(SSDB *ssdb, SSDB *meta, const char *ip, int port, bool is_mirror){
	thread_quit = false;
	this->ssdb = ssdb;
	this->meta = meta;
	this->status = DISCONNECTED;
	this->master_ip = std::string(ip);
	this->master_port = port;
	this->is_mirror = is_mirror;
	if(this->is_mirror){
		this->log_type = BinlogType::MIRROR;
	}else{
		this->log_type = BinlogType::SYNC;
	}
	
	{
		char buf[128];
		snprintf(buf, sizeof(buf), "%s|%d", master_ip.c_str(), master_port);
		this->set_id(buf);
	}
	
	this->link = NULL;
	this->last_seq = 0;
	this->last_key = "";
	this->connect_retry = 0;
	
	this->copy_count = 0;
	this->sync_count = 0;
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

std::string Slave::stats() const{
	std::string s;
	s.append("slaveof " + master_ip + ":" + str(master_port) + "\n");
	s.append("    id         : " + id_ + "\n");
	if(is_mirror){
		s.append("    type       : mirror\n");
	}else{
		s.append("    type       : sync\n");
	}

	s.append("    status     : ");
	switch(status){
	case DISCONNECTED:
		s.append("DISCONNECTED\n");
		break;
	case INIT:
		s.append("INIT\n");
		break;
	case COPY:
		s.append("COPY\n");
		break;
	case SYNC:
		s.append("SYNC\n");
		break;
	}

	s.append("    last_seq   : " + str(last_seq) + "\n");
	s.append("    copy_count : " + str(copy_count) + "\n");
	s.append("    sync_count : " + str(sync_count) + "");
	return s;
}

void Slave::start(){
	migrate_old_status();
	load_status();
	log_debug("last_seq: %" PRIu64 ", last_key: %s",
		last_seq, hexmem(last_key.data(), last_key.size()).c_str());

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

void Slave::set_id(const std::string &id){
	this->id_ = id;
}

void Slave::migrate_old_status(){
	std::string old_key = "new.slave.status|" + this->id_;
	std::string val;
	int old_found = meta->raw_get(old_key, &val);
	if(!old_found){
		return;
	}
	if(val.size() < sizeof(uint64_t)){
		log_error("invalid format of status");
		return;
	}
	last_seq = *((uint64_t *)(val.data()));
	last_key.assign(val.data() + sizeof(uint64_t), val.size() - sizeof(uint64_t));
	// migrate old status
	log_info("migrate old version slave status to new format, last_seq: %" PRIu64 ", last_key: %s",
		last_seq, hexmem(last_key.data(), last_key.size()).c_str());
	
	save_status();
	if(meta->raw_del(old_key) == -1){
		log_fatal("meta db error!");
		exit(1);
	}
}

std::string Slave::status_key(){
	static std::string key;
	if(key.empty()){
		key = "slave.status." + this->id_;
	}
	return key;
}

void Slave::load_status(){
	std::string key;
	std::string seq;
	meta->hget(status_key(), "last_key", &key);
	meta->hget(status_key(), "last_seq", &seq);
	if(!key.empty()){
		this->last_key = key;
	}
	if(!seq.empty()){
		this->last_seq = str_to_uint64(seq);
	}
}

void Slave::save_status(){
	std::string seq = str(this->last_seq);
	meta->hset(status_key(), "last_key", this->last_key);
	meta->hset(status_key(), "last_seq", seq);
}

int Slave::connect(){
	const char *ip = this->master_ip.c_str();
	int port = this->master_port;
	
	if(++connect_retry % 50 == 1){
		log_info("[%s][%d] connecting to master at %s:%d...", this->id_.c_str(), (connect_retry-1)/50, ip, port);
		link = Link::connect(ip, port);
		if(link == NULL){
			log_error("[%s]failed to connect to master: %s:%d! %s", this->id_.c_str(), ip, port, strerror(errno));
			goto err;
		}else{
			status = INIT;
			connect_retry = 0;
			const char *type = is_mirror? "mirror" : "sync";
			
			if(!this->auth.empty()){
				const std::vector<Bytes> *resp;
				resp = link->request("auth", this->auth);
				if(resp->empty() || resp->at(0) != "ok"){
					log_error("auth error");
					delete link;
					link = NULL;
					sleep(1);
					return -1;
				}
			}
			
			link->send("sync140", str(this->last_seq), this->last_key, type);
			if(link->flush() == -1){
				log_error("[%s] network error", this->id_.c_str());
				delete link;
				link = NULL;
				goto err;
			}
			log_info("[%s] ready to receive binlogs", this->id_.c_str());
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
	
#define RECV_TIMEOUT		200
#define MAX_RECV_TIMEOUT	15 * 1000
#define MAX_RECV_IDLE		MAX_RECV_TIMEOUT/RECV_TIMEOUT

	while(!slave->thread_quit){
		if(reconnect){
			slave->status = DISCONNECTED;
			reconnect = false;
			select.del(slave->link->fd());
			delete slave->link;
			slave->link = NULL;
			sleep(1);
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
			log_error("events.wait error: %s", strerror(errno));
			sleep(1);
			continue;
		}else if(events->empty()){
			if(idle++ >= MAX_RECV_IDLE){
				log_error("the master hasn't responsed for awhile, reconnect...");
				idle = 0;
				reconnect = true;
			}
			continue;
		}
		idle = 0;

		if(slave->link->read() <= 0){
			log_error("link.read error: %s, reconnecting to master", strerror(errno));
			reconnect = true;
			continue;
		}

		while(1){
			req = slave->link->recv();
			if(req == NULL){
				log_error("link.recv error: %s, reconnecting to master", strerror(errno));
				reconnect = true;
				break;
			}else if(req->empty()){
				break;
			}else if(req->at(0) == "noauth"){
				log_error("authentication required");
				reconnect = true;
				sleep(1);
				break;
			}else{
				if(slave->proc(*req) == -1){
					goto err;
				}
			}
		}
	} // end while
	log_info("Slave thread quit");
	return (void *)NULL;

err:
	log_fatal("Slave thread exit unexpectedly");
	exit(0);
	return (void *)NULL;;
}

int Slave::proc(const std::vector<Bytes> &req){
	Binlog log;
	if(log.load(req[0]) == -1){
		log_error("invalid binlog!");
		return 0;
	}
	const char *sync_type = this->is_mirror? "mirror" : "sync";
	switch(log.type()){
		case BinlogType::NOOP:
			return this->proc_noop(log, req);
			break;
		case BinlogType::COPY:{
			status = COPY;
			if(req.size() >= 2){
				log_debug("[%s] %s [%d]", sync_type, log.dumps().c_str(), req[1].size());
			}else{
				log_debug("[%s] %s", sync_type, log.dumps().c_str());
			}
			this->proc_copy(log, req);
			break;
		}
		case BinlogType::SYNC:
		case BinlogType::MIRROR:{
			status = SYNC;
			if(++sync_count % 1000 == 1){
				log_info("sync_count: %" PRIu64 ", last_seq: %" PRIu64 ", seq: %" PRIu64 "",
					sync_count, this->last_seq, log.seq());
			}
			if(req.size() >= 2){
				log_debug("[%s] %s [%d]", sync_type, log.dumps().c_str(), req[1].size());
			}else{
				log_debug("[%s] %s", sync_type, log.dumps().c_str());
			}
			this->proc_sync(log, req);
			break;
		}
		default:
			break;
	}
	return 0;
}

int Slave::proc_noop(const Binlog &log, const std::vector<Bytes> &req){
	uint64_t seq = log.seq();
	if(this->last_seq != seq){
		log_debug("noop last_seq: %" PRIu64 ", seq: %" PRIu64 "", this->last_seq, seq);
		this->last_seq = seq;
		this->save_status();
	}
	return 0;
}

int Slave::proc_copy(const Binlog &log, const std::vector<Bytes> &req){
	switch(log.cmd()){
		case BinlogCommand::BEGIN:
			log_info("copy begin");
			log_info("start flushdb...");
			this->last_seq = 0;
			this->last_key = "";
			this->save_status();
			ssdb->flushdb();
			log_info("end flushdb.");
			break;
		case BinlogCommand::END:
			log_info("copy end, copy_count: %" PRIu64 ", last_seq: %" PRIu64 ", seq: %" PRIu64,
				copy_count, this->last_seq, log.seq());
			this->status = SYNC;
			this->last_key = "";
			this->save_status();
			break;
		default:
			if(++copy_count % 1000 == 1){
				log_info("copy_count: %" PRIu64 ", last_seq: %" PRIu64 ", seq: %" PRIu64 "",
					copy_count, this->last_seq, log.seq());
			}
			return proc_sync(log, req);
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
				log_trace("set %s", hexmem(key.data(), key.size()).c_str());
				if(ssdb->set(key, req[1], log_type) == -1){
					return -1;
				}
			}
			break;
		case BinlogCommand::KDEL:
			{
				std::string key;
				if(decode_kv_key(log.key(), &key) == -1){
					break;
				}
				log_trace("del %s", hexmem(key.data(), key.size()).c_str());
				if(ssdb->del(key, log_type) == -1){
					return -1;
				}
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
				log_trace("hset %s %s",
					hexmem(name.data(), name.size()).c_str(),
					hexmem(key.data(), key.size()).c_str());
				if(ssdb->hset(name, key, req[1], log_type) == -1){
					return -1;
				}
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
				if(ssdb->hdel(name, key, log_type) == -1){
					return -1;
				}
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
				log_trace("zset %s %s",
					hexmem(name.data(), name.size()).c_str(),
					hexmem(key.data(), key.size()).c_str());
				if(ssdb->zset(name, key, req[1], log_type) == -1){
					return -1;
				}
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
				if(ssdb->zdel(name, key, log_type) == -1){
					return -1;
				}
			}
			break;
		case BinlogCommand::QSET:
		case BinlogCommand::QPUSH_BACK:
		case BinlogCommand::QPUSH_FRONT:
			{
				if(req.size() != 2){
					break;
				}
				std::string name;
				uint64_t seq;
				if(decode_qitem_key(log.key(), &name, &seq) == -1){
					break;
				}
				if(seq < QITEM_MIN_SEQ || seq > QITEM_MAX_SEQ){
					break;
				}
				int ret;
				if(log.cmd() == BinlogCommand::QSET){
					log_trace("qset %s %" PRIu64 "", hexmem(name.data(), name.size()).c_str(), seq);
					ret = ssdb->qset_by_seq(name, seq, req[1], log_type);
				}else if(log.cmd() == BinlogCommand::QPUSH_BACK){
					log_trace("qpush_back %s", hexmem(name.data(), name.size()).c_str());
					ret = ssdb->qpush_back(name, req[1], log_type);
				}else{
					log_trace("qpush_front %s", hexmem(name.data(), name.size()).c_str());
					ret = ssdb->qpush_front(name, req[1], log_type);
				}
				if(ret == -1){
					return -1;
				}
			}
			break;
		case BinlogCommand::QPOP_BACK:
		case BinlogCommand::QPOP_FRONT:
			{
				int ret;
				const Bytes name = log.key();
				std::string tmp;
				if(log.cmd() == BinlogCommand::QPOP_BACK){
					log_trace("qpop_back %s", hexmem(name.data(), name.size()).c_str());
					ret = ssdb->qpop_back(name, &tmp, log_type);
				}else{
					log_trace("qpop_front %s", hexmem(name.data(), name.size()).c_str());
					ret = ssdb->qpop_front(name, &tmp, log_type);
				}
				if(ret == -1){
					return -1;
				}
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

