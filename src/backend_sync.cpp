#include <pthread.h>
#include <assert.h>
#include <errno.h>
#include <string>
#include "backend_sync.h"
#include "util/strings.h"

BackendSync::BackendSync(const SSDB *ssdb){
	thread_quit = false;
	this->ssdb = ssdb;
}

BackendSync::~BackendSync(){
	thread_quit = true;
	int retry = 0;
	int MAX_RETRY = 50;
	while(retry++ < MAX_RETRY){
		// there is something wrong that sleep makes other threads
		// unable to acquire the mutex
		{
			Locking l(&mutex);
			if(workers.empty()){
				break;
			}
		}
		usleep(100 * 1000);
	}
	if(retry >= MAX_RETRY){
		log_info("Backend worker not exit expectedly");
	}
	log_debug("BackendSync finalized");
}

void BackendSync::proc(const Link *link){
	log_info("accept sync client");
	struct run_arg *arg = new run_arg();
	arg->link = link;
	arg->backend = this;

	pthread_t tid;
	int err = pthread_create(&tid, NULL, &BackendSync::_run_thread, arg);
	if(err != 0){
		log_error("can't create thread: %s", strerror(err));
		delete link;
	}
	Locking l(&mutex);
	workers[tid] = tid;
}

void* BackendSync::_run_thread(void *arg){
	struct run_arg *p = (struct run_arg*)arg;
	BackendSync *backend = (BackendSync *)p->backend;
	Link *link = (Link *)p->link;
	delete p;

	//
	link->noblock(false);

	SSDB *ssdb = (SSDB *)backend->ssdb;
	BinlogQueue *logs = ssdb->binlogs;

	Client client(backend);
	client.link = link;
	client.init();

	int idle = 0;
	while(!backend->thread_quit){
		// TODO: test
		//usleep(2000 * 1000);
		
		if(client.status == Client::OUT_OF_SYNC){
			client.reset();
			continue;
		}
		
		bool is_empty = true;
		if(client.status == Client::COPY){
			if(client.copy()){
				is_empty = false;
			}
		}
		if(client.sync(logs)){
			is_empty = false;
		}
		if(is_empty){
			if(idle == 10){
				idle = 0;
				client.noop();
			}else{
				idle ++;
				// sleep longer to reduce logs.find
				usleep(300 * 1000);
			}
		}else{
			idle = 0;
		}

		if(link->flush() == -1){
			log_error("fd: %d, send error: %s", link->fd(), strerror(errno));
			break;
		}
	}

	log_info("Sync Client quit, fd: %d, delete link", link->fd());
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
	if(last_key == "" && last_seq != 0){
		log_info("[%s]fd: %d, sync, seq: %"PRIu64", key: '%s'",
			type,
			link->fd(),
			last_seq, hexmem(last_key.data(), last_key.size()).c_str()
			);
		this->status = Client::SYNC;
	}else{
		// a slave must reset its last_key when receiving 'copy_end' command
		log_info("[%s]fd: %d, copy recover, seq: %"PRIu64", key: '%s'",
			type,
			link->fd(),
			last_seq, hexmem(last_key.data(), last_key.size()).c_str()
			);
		this->status = Client::COPY;
	}
}

void BackendSync::Client::reset(){
	log_info("fd: %d, copy begin", link->fd());
	this->status = Client::COPY;
	this->last_seq = 0;
	this->last_key = "";

	Binlog log(this->last_seq, BinlogType::COPY, BinlogCommand::BEGIN, "");
	log_trace("fd: %d, %s", link->fd(), log.dumps().c_str());
	link->send(log.repr(), "copy_begin");
}

void BackendSync::Client::noop(){
	this->last_noop_seq = this->last_seq;
	Binlog noop(this->last_seq, BinlogType::NOOP, BinlogCommand::NONE, "");
	//log_trace("fd: %d, %s", link->fd(), log.dumps().c_str());
	link->send(noop.repr());
}

int BackendSync::Client::copy(){
	if(this->iter == NULL){
		log_debug("new iterator, last_key: '%s'", hexmem(last_key.data(), last_key.size()).c_str());
		this->iter = backend->ssdb->iterator(this->last_key, "", -1);
	}
	for(int i=0; i<1000; i++){
		if(!iter->next()){
			log_info("fd: %d, copy end", link->fd());
			this->status = Client::SYNC;
			delete this->iter;
			this->iter = NULL;

			Binlog log(this->last_seq, BinlogType::COPY, BinlogCommand::END, "");
			log_trace("fd: %d, %s", link->fd(), log.dumps().c_str());
			link->send(log.repr(), "copy_end");
			break;
		}else{
			Bytes key = iter->key();
			Bytes val = iter->val();
			this->last_key = key.String();
			
			if(key.size() == 0){
				continue;
			}
			
			char cmd = 0;
			char data_type = key.data()[0];
			if(data_type == DataType::KV){
				cmd = BinlogCommand::KSET;
			}else if(data_type == DataType::HASH){
				cmd = BinlogCommand::HSET;
			}else if(data_type == DataType::ZSET){
				cmd = BinlogCommand::ZSET;
			}else{
				continue;
			}
			
			Binlog log(this->last_seq, BinlogType::COPY, cmd, key.Slice());
			log_trace("fd: %d, %s", link->fd(), log.dumps().c_str());
			link->send(log.repr(), val);
			//if(link->output->size() > 1024 * 1024){
			break;
			//}
		}
	}
	
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
		// writes that are out of copied range will be discarded.
		if(this->status == Client::COPY && log.key() > this->last_key){
			log_trace("fd: %d, last_key: '%s', drop: %s",
				link->fd(),
				hexmem(this->last_key.data(), this->last_key.size()).c_str(),
				log.dumps().c_str());
			this->last_seq = log.seq();
			//if(this->iter){
			//	delete this->iter;
			//	this->iter = NULL;
			//}
			continue;
		}
		if(this->last_seq != 0 && log.seq() != expect_seq){
			log_warn("fd: %d, OUT_OF_SYNC! seq: %"PRIu64", last_seq: %"PRIu64"",
				link->fd(),
				log.seq(),
				expect_seq
				);
			this->status = Client::OUT_OF_SYNC;
			return 1;
		}
		break;
	}
	
	// update last_seq
	this->last_seq = log.seq();

	char type = log.type();
	if(type == BinlogType::MIRROR && this->is_mirror){
		if(this->last_seq - this->last_noop_seq >= 1000){
			this->noop();
			return 1;
		}else{
			return 0;
		}
	}

	int ret = 0;
	std::string val;
	switch(log.cmd()){
		case BinlogCommand::KSET:
		case BinlogCommand::HSET:
		case BinlogCommand::ZSET:
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
			log_trace("fd: %d, %s", link->fd(), log.dumps().c_str());
			link->send(log.repr());
			break;
	}
	return 1;
}
