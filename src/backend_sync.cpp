/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include <pthread.h>
#include <assert.h>
#include <errno.h>
#include <string>
#include "backend_sync.h"
#include "util/log.h"
#include "util/strings.h"

BackendSync::BackendSync(SSDBImpl *ssdb, int sync_speed){
	thread_quit = false;
	this->ssdb = ssdb;
	this->sync_speed = sync_speed;
}

BackendSync::~BackendSync(){
	thread_quit = true;
	int retry = 0;
	int MAX_RETRY = 100;
	while(retry++ < MAX_RETRY){
		// there is something wrong that sleep makes other threads
		// unable to acquire the mutex
		{
			Locking l(&mutex);
			if(workers.empty()){
				break;
			}
		}
		usleep(50 * 1000);
	}
	if(retry >= MAX_RETRY){
		log_info("Backend worker not exit expectedly");
	}
	log_debug("BackendSync finalized");
}

std::vector<std::string> BackendSync::stats(){
	std::vector<std::string> ret;
	std::map<pthread_t, Client *>::iterator it;

	Locking l(&mutex);
	for(it = workers.begin(); it != workers.end(); it++){
		Client *client = it->second;
		ret.push_back(client->stats());
	}
	return ret;
}

void BackendSync::proc(const Link *link){
	log_info("fd: %d, accept sync client", link->fd());
	struct run_arg *arg = new run_arg();
	arg->link = link;
	arg->backend = this;

	pthread_t tid;
	int err = pthread_create(&tid, NULL, &BackendSync::_run_thread, arg);
	if(err != 0){
		log_error("can't create thread: %s", strerror(err));
		delete link;
	}
}

void* BackendSync::_run_thread(void *arg){
	pthread_detach(pthread_self());
	struct run_arg *p = (struct run_arg*)arg;
	BackendSync *backend = (BackendSync *)p->backend;
	Link *link = (Link *)p->link;
	delete p;

	// set Link non block
	link->noblock(false);

	SSDBImpl *ssdb = (SSDBImpl *)backend->ssdb;
	BinlogQueue *logs = ssdb->binlogs;

	Client client(backend);
	client.link = link;
	client.init();

	{
		pthread_t tid = pthread_self();
		Locking l(&backend->mutex);
		backend->workers[tid] = &client;
	}

// sleep longer to reduce logs.find
#define TICK_INTERVAL_MS	300
#define NOOP_IDLES			(3000/TICK_INTERVAL_MS)

	int idle = 0;
	while(!backend->thread_quit){
		// TODO: test
		//usleep(2000 * 1000);
		
		if(client.status == Client::OUT_OF_SYNC){
			client.reset();
			continue;
		}
		
		bool is_empty = true;
		// WARN: MUST do first sync() before first copy(), because
		// sync() will refresh last_seq, and copy() will not
		if(client.sync(logs)){
			is_empty = false;
		}
		if(client.status == Client::COPY){
			if(client.copy()){
				is_empty = false;
			}
		}
		if(is_empty){
			if(idle >= NOOP_IDLES){
				idle = 0;
				client.noop();
			}else{
				idle ++;
				usleep(TICK_INTERVAL_MS * 1000);
			}
		}else{
			idle = 0;
		}

		float data_size_mb = link->output->size() / 1024.0 / 1024.0;
		if(link->flush() == -1){
			log_info("%s:%d fd: %d, send error: %s", link->remote_ip, link->remote_port, link->fd(), strerror(errno));
			break;
		}
		if(backend->sync_speed > 0){
			usleep((data_size_mb / backend->sync_speed) * 1000 * 1000);
		}
	}

	log_info("Sync Client quit, %s:%d fd: %d, delete link", link->remote_ip, link->remote_port, link->fd());
	delete link;

	Locking l(&backend->mutex);
	backend->workers.erase(pthread_self());
	return (void *)NULL;
}


/* Client */

BackendSync::Client::Client(const BackendSync *backend){
	status = Client::INIT;
	this->backend = backend;
	link = NULL;
	last_seq = 0;
	last_noop_seq = 0;
	last_key = "";
	is_mirror = false;
	iter = NULL;
}

BackendSync::Client::~Client(){
	if(iter){
		delete iter;
		iter = NULL;
	}
}

std::string BackendSync::Client::stats(){
	std::string s;
	s.append("client " + str(link->remote_ip) + ":" + str(link->remote_port) + "\n");
	s.append("    type     : ");
	if(is_mirror){
		s.append("mirror\n");
	}else{
		s.append("sync\n");
	}
	
	s.append("    status   : ");
	switch(status){
	case INIT:
		s.append("INIT\n");
		break;
	case OUT_OF_SYNC:
		s.append("OUT_OF_SYNC\n");
		break;
	case COPY:
		s.append("COPY\n");
		break;
	case SYNC:
		s.append("SYNC\n");
		break;
	}
	
	s.append("    last_seq : " + str(last_seq) + "");
	return s;
}

void BackendSync::Client::init(){
	const std::vector<Bytes> *req = this->link->last_recv();
	last_seq = 0;
	if(req->size() > 1){
		last_seq = req->at(1).Uint64();
	}
	last_key = "";
	if(req->size() > 2){
		last_key = req->at(2).String();
	}
	// is_mirror
	if(req->size() > 3){
		if(req->at(3).String() == "mirror"){
			is_mirror = true;
		}
	}
	const char *type = is_mirror? "mirror" : "sync";
	// a slave must reset its last_key when receiving 'copy_end' command
	if(last_key == "" && last_seq != 0){
		log_info("[%s] %s:%d fd: %d, sync recover, seq: %" PRIu64 ", key: '%s'",
			type,
			link->remote_ip, link->remote_port,
			link->fd(),
			last_seq, hexmem(last_key.data(), last_key.size()).c_str()
			);
		this->status = Client::SYNC;
		
		Binlog log(this->last_seq, BinlogType::COPY, BinlogCommand::END, "");
		log_trace("fd: %d, %s", link->fd(), log.dumps().c_str());
		link->send(log.repr(), "copy_end");
	}else if(last_key == "" && last_seq == 0){
		log_info("[%s] %s:%d fd: %d, copy begin, seq: %" PRIu64 ", key: '%s'",
			type,
			link->remote_ip, link->remote_port,
			link->fd(),
			last_seq, hexmem(last_key.data(), last_key.size()).c_str()
			);
		this->reset();
	}else{
		log_info("[%s] %s:%d fd: %d, copy recover, seq: %" PRIu64 ", key: '%s'",
			type,
			link->remote_ip, link->remote_port,
			link->fd(),
			last_seq, hexmem(last_key.data(), last_key.size()).c_str()
			);
		this->status = Client::COPY;
	}
}

void BackendSync::Client::reset(){
	log_info("%s:%d fd: %d, copy begin", link->remote_ip, link->remote_port, link->fd());
	this->status = Client::COPY;
	this->last_seq = 0;
	this->last_key = "";

	Binlog log(this->last_seq, BinlogType::COPY, BinlogCommand::BEGIN, "");
	log_trace("fd: %d, %s", link->fd(), log.dumps().c_str());
	link->send(log.repr(), "copy_begin");
}

void BackendSync::Client::noop(){
	uint64_t seq;
	if(this->status == Client::COPY && this->last_key.empty()){
		seq = 0;
	}else{
		seq = this->last_seq;
		this->last_noop_seq = this->last_seq;
	}
	Binlog noop(seq, BinlogType::NOOP, BinlogCommand::NONE, "");
	//log_debug("fd: %d, %s", link->fd(), noop.dumps().c_str());
	link->send(noop.repr());
}

int BackendSync::Client::copy(){
	if(this->iter == NULL){
		log_info("new iterator, last_key: '%s'", hexmem(last_key.data(), last_key.size()).c_str());
		std::string key = this->last_key;
		if(this->last_key.empty()){
			key.push_back(DataType::MIN_PREFIX);
		}
		this->iter = backend->ssdb->iterator(key, "", -1);
		log_info("iterator created, last_key: '%s'", hexmem(last_key.data(), last_key.size()).c_str());
	}
	int ret = 0;
	int iterate_count = 0;
	int64_t stime = time_ms();
	while(true){
		// Prevent copy() from blocking too long
		if(++iterate_count > 1000 || link->output->size() > 2 * 1024 * 1024){
			break;
		}
		if(time_ms() - stime > 3000){
			log_info("copy blocks too long, flush");
			break;
		}
		
		if(!iter->next()){
			goto copy_end;
		}
		Bytes key = iter->key();
		if(key.size() == 0){
			continue;
		}
		// finish copying all valid data types
		if(key.data()[0] > DataType::MAX_PREFIX){
			goto copy_end;
		}
		Bytes val = iter->val();
		this->last_key = key.String();
			
		char cmd = 0;
		char data_type = key.data()[0];
		if(data_type == DataType::KV){
			cmd = BinlogCommand::KSET;
		}else if(data_type == DataType::HASH){
			cmd = BinlogCommand::HSET;
		}else if(data_type == DataType::ZSET){
			cmd = BinlogCommand::ZSET;
		}else if(data_type == DataType::QUEUE){
			cmd = BinlogCommand::QPUSH_BACK;
		}else{
			continue;
		}
		
		ret = 1;
		
		Binlog log(this->last_seq, BinlogType::COPY, cmd, slice(key));
		log_trace("fd: %d, %s", link->fd(), log.dumps().c_str());
		link->send(log.repr(), val);
	}
	return ret;

copy_end:		
	log_info("%s:%d fd: %d, copy end", link->remote_ip, link->remote_port, link->fd());
	this->status = Client::SYNC;
	delete this->iter;
	this->iter = NULL;

	Binlog log(this->last_seq, BinlogType::COPY, BinlogCommand::END, "");
	log_trace("fd: %d, %s", link->fd(), log.dumps().c_str());
	link->send(log.repr(), "copy_end");
	return 1;
}

int BackendSync::Client::sync(BinlogQueue *logs){
	Binlog log;
	while(1){
		int ret = 0;
		uint64_t expect_seq = this->last_seq + 1;
		if(this->status == Client::COPY && this->last_seq == 0){
			ret = logs->find_last(&log);
		}else{
			ret = logs->find_next(expect_seq, &log);
		}
		if(ret == 0){
			return 0;
		}
		if(this->status == Client::COPY && log.key() > this->last_key){
			log_debug("fd: %d, last_key: '%s', drop: %s",
				link->fd(),
				hexmem(this->last_key.data(), this->last_key.size()).c_str(),
				log.dumps().c_str());
			this->last_seq = log.seq();
			// WARN: When there are writes behind last_key, we MUST create
			// a new iterator, because iterator will not know this key.
			// Because iterator ONLY iterates throught keys written before
			// iterator is created.
			if(this->iter){
				delete this->iter;
				this->iter = NULL;
			}
			continue;
		}
		if(this->last_seq != 0 && log.seq() != expect_seq){
			log_warn("%s:%d fd: %d, OUT_OF_SYNC! log.seq: %" PRIu64 ", expect_seq: %" PRIu64 "",
				link->remote_ip, link->remote_port,
				link->fd(),
				log.seq(),
				expect_seq
				);
			this->status = Client::OUT_OF_SYNC;
			return 1;
		}
	
		// update last_seq
		this->last_seq = log.seq();

		char type = log.type();
		if(type == BinlogType::MIRROR && this->is_mirror){
			if(this->last_seq - this->last_noop_seq >= 1000){
				this->noop();
				return 1;
			}else{
				continue;
			}
		}
		
		break;
	}

	int ret = 0;
	std::string val;
	switch(log.cmd()){
		case BinlogCommand::KSET:
		case BinlogCommand::HSET:
		case BinlogCommand::ZSET:
		case BinlogCommand::QSET:
		case BinlogCommand::QPUSH_BACK:
		case BinlogCommand::QPUSH_FRONT:
			ret = backend->ssdb->raw_get(log.key(), &val);
			if(ret == -1){
				log_error("fd: %d, raw_get error!", link->fd());
			}else if(ret == 0){
				//log_debug("%s", hexmem(log.key().data(), log.key().size()).c_str());
				log_trace("fd: %d, skip not found: %s", link->fd(), log.dumps().c_str());
			}else{
				log_trace("fd: %d, %s", link->fd(), log.dumps().c_str());
				link->send(log.repr(), val);
			}
			break;
		case BinlogCommand::KDEL:
		case BinlogCommand::HDEL:
		case BinlogCommand::ZDEL:
		case BinlogCommand::QPOP_BACK:
		case BinlogCommand::QPOP_FRONT:
			log_trace("fd: %d, %s", link->fd(), log.dumps().c_str());
			link->send(log.repr());
			break;
	}
	return 1;
}
