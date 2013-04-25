#include <pthread.h>
#include <assert.h>
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
		// TEST: simulate slow network
		#ifndef NDEBUG
		//usleep(1000 * 1000);
		#endif

		if(client.status == Client::OUT_OF_SYNC){
			client.reset_sync();
		}

		bool is_empty = true;
		if(client.dump()){
			is_empty = false;
		}
		if(client.sync(logs)){
			is_empty = false;
		}
		if(is_empty){
			idle ++;
			if(idle == 30){
				idle = 0;
				client.last_noop_seq = client.last_seq;
				Binlog noop(client.last_seq, BinlogType::NOOP, BinlogCommand::NONE, "");
				//log_trace("fd: %d, %s", link->fd(), noop.dumps().c_str());
				if(link->send(noop.repr()) == -1){
					log_debug("fd: %d, send error", link->fd());
					break;
				}
			}
			// sleep longer to reduce logs.find
			usleep(200 * 1000);
		}else{
			idle = 0;
		}

		if(link->flush() == -1){
			log_debug("fd: %d, send error", link->fd());
			break;
		}
	}

	log_info("SyncClient quit, fd: %d, delete link", link->fd());
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
	iter = NULL;
	is_mirror = false;
}

BackendSync::Client::~Client(){
	if(iter){
		delete iter;
	}
}

void BackendSync::Client::init(){
	const std::vector<Bytes>* req = this->link->last_recv();
	last_seq = 0;
	if(req->size() > 1){
		last_seq = req->at(1).Uint64();
	}
	last_key = "";
	if(req->size() > 2){
		last_key = req->at(2).String();
	}
	// is_mirror
	std::string type = "sync";
	if(req->size() > 3){
		if(req->at(3).String() == "mirror"){
			type = "mirror";
			is_mirror = true;
		}
	}
	if(!is_mirror && last_seq == 0){
		// in case that slave has an error that last_key is not empty
		log_info("fd: %d, new slave, make a full dumping", link->fd());
		this->reset_sync();
	}else if(!is_mirror && last_key != ""){
		// a slave must reset its last_key when receiving 'dump_end' command
		log_info("fd: %d, dump recover, seq: %llu, key: %s",
			link->fd(),
			last_seq, hexmem(last_key.data(), last_key.size()).c_str()
			);
		std::string end = "";
		this->iter = backend->ssdb->iterator(last_key, end, -1);
		this->status = Client::DUMP;
	}else{
		log_info("[%s]fd: %d, sync recover, seq: %llu, key: %s",
			type.c_str(),
			link->fd(),
			last_seq, hexmem(last_key.data(), last_key.size()).c_str()
			);
		this->status = Client::SYNC;
	}
}

void BackendSync::Client::reset_sync(){
	log_info("fd: %d, dump begin", link->fd());
	if(this->iter){
		delete this->iter;
		this->iter = NULL;
	}
	std::string start = "";
	std::string end = "";
	if(!is_mirror){
		this->iter = backend->ssdb->iterator(start, end, -1);
	}
	this->status = Client::DUMP;
	this->last_seq = 0;
	this->last_key = "";

	Buffer *output = link->output;
	Binlog log(0, BinlogType::DUMP, BinlogCommand::BEGIN, "");
	log_trace("fd: %d, %s", link->fd(), log.dumps().c_str());
	output->append_record(log.repr());
	output->append_record("dump_begin");
	output->append('\n');
}

int BackendSync::Client::dump(){
	if(this->iter == NULL){
		return 0;
	}
	
	Buffer *output = link->output;
	for(int i=0; i<1000; i++){
		if(!this->iter->next()){
			log_info("fd: %d, dump end", link->fd());
			delete this->iter;
			this->iter = NULL;
			this->status = Client::SYNC;
		
			Binlog log(0, BinlogType::DUMP, BinlogCommand::END, "");
			log_trace("fd: %d, %s", link->fd(), log.dumps().c_str());
			output->append_record(log.repr());
			output->append_record("dump_end");
			output->append('\n');
			break;
		}else{
			Bytes key = this->iter->key();
			Bytes val = this->iter->val();

			if(key.size() > 0){
				char cmd = -1;
				char data_type = key.data()[0];
				if(data_type == DataType::KV){
					cmd = BinlogCommand::KSET;
				}else if(data_type == DataType::HASH){
					cmd = BinlogCommand::HSET;
				}else if(data_type == DataType::ZSET){
					cmd = BinlogCommand::ZSET;
				}else{
					// ignore
				}
				if(cmd > 0){
					this->last_key = key.String();
					Binlog log(this->last_seq, BinlogType::DUMP, cmd, key.Slice());
					log_trace("fd: %d, %s", link->fd(), log.dumps().c_str());
					output->append_record(log.repr());
					output->append_record(val);
					output->append('\n');
					break;
				}
			}
		}
	}
	return 1;
}

int BackendSync::Client::sync(BinlogQueue *logs){
	Buffer *output = link->output;

	uint64_t expect_seq = this->last_seq + 1;
	Binlog log;
	int ret;
	if(this->status == Client::DUMP && this->last_seq == 0){
		ret = logs->find_last(&log);
	}else{
		ret = logs->find_next(expect_seq, &log);
	}
	if(ret == 0){
		return 0;
	}
	//log_trace("fd: %d, %s", link->fd(), log.dumps().c_str());
	
	// writes that are out of dumped range will be discarded.
	if(this->iter && log.key() > this->last_key){
		// update last_seq
		this->last_seq = log.seq();

		log_trace("fd: %d, last_key: '%s', drop: %s",
			link->fd(),
			hexmem(this->last_key.data(), this->last_key.size()).c_str(),
			log.dumps().c_str());
		return 1;
	}

	if(this->last_seq != 0 && log.seq() != expect_seq){
		log_warn("fd: %d, OUT_OF_SYNC! seq: %llu, last_seq: %llu",
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
		if(this->last_seq - this->last_noop_seq >= 100){
			this->last_noop_seq = this->last_seq;
			Binlog noop(this->last_seq, BinlogType::NOOP, BinlogCommand::NONE, "");
			//log_trace("fd: %d, %s", link->fd(), log.dumps().c_str());
			output->append_record(noop.repr());
			output->append('\n');
		}
	}else{
		int ret;
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

					output->append_record(log.repr());
					output->append_record(val);
					output->append('\n');
				}
				break;
			case BinlogCommand::KDEL:
			case BinlogCommand::HDEL:
			case BinlogCommand::ZDEL:
				log_trace("fd: %d, %s", link->fd(), log.dumps().c_str());
				output->append_record(log.repr());
				output->append('\n');
				break;
		}
	}
	return 1;
}
