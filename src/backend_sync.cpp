#include <pthread.h>
#include <assert.h>
#include <string>
#include "backend_sync.h"
#include "util/strings.h"
#include "repl.h"

BackendSync::BackendSync(const SSDB *ssdb){
	this->ssdb = ssdb;
}

BackendSync::~BackendSync(){
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
}

void* BackendSync::_run_thread(void *arg){
	struct run_arg *p = (struct run_arg*)arg;
	const BackendSync *backend = p->backend;
	Link *link = (Link *)p->link;
	delete p;

	//
	link->noblock(false);

	SSDB *ssdb = (SSDB *)backend->ssdb;
	SyncLogQueue *logs = ssdb->replication->logs;

	Client client(backend);
	client.link = link;
	client.init();

	int idle = 0;
	while(1){
		// TEST: simulate slow network
		usleep(1000 * 1000);

		if(client.status == Client::OUT_OF_SYNC){
			// TODO: tell slave to clear database
			client.re_sync();
		}

		bool is_empty = true;
		if(client.iter){
			is_empty = false;
			client.dump();
		}
		if(client.sync(logs)){
			is_empty = false;
		}
		if(is_empty){
			idle ++;
			if(idle == 4){
				idle = 0;
				if(link->send("noop") == -1){
					log_debug("fd: %d, send error", link->fd());
					break;
				}
			}
			// sleep longer to reduce logs.find
			usleep(500 * 1000);
		}else{
			idle = 0;
		}

		if(link->flush() == -1){
			log_debug("fd: %d, send error", link->fd());
			break;
		}
	}

	log_info("fd: %d, delete link", link->fd());
	delete link;
	return (void *)NULL;
}


/* Client */

BackendSync::Client::Client(const BackendSync *backend){
	status = Client::INIT;
	this->backend = backend;
	link = NULL;
	next_seq = 0;
	last_key = "";
	iter = NULL;
}

BackendSync::Client::~Client(){
	if(iter){
		delete iter;
	}
}

void BackendSync::Client::init(){
	const std::vector<Bytes>* req = this->link->last_recv();
	next_seq = 0;
	if(req->size() > 1){
		next_seq = req->at(1).Uint64();
	}
	last_key = "";
	if(req->size() > 2){
		last_key = req->at(2).String();
	}
	if(next_seq == 0 || last_key != ""){
		if(next_seq == 0){
			// in case that slave has an error that last_key is not empty
			last_key = "";
			log_info("fd: %d, new slave, make a full dumping", link->fd());
		}else{
			// last_key != ""
			// a slave must reset its last_key when receiving 'dump_end' command
			log_info("fd: %d, recover dumping", link->fd());
		}
		std::string end = "";
		int limit = 2147483647;
		this->iter = backend->ssdb->iterator(last_key, end, limit);
		this->status = Client::DUMP;
	}else{
		this->status = Client::SYNC;
	}
}

void BackendSync::Client::re_sync(){
	if(this->iter){
		delete this->iter;
	}
	std::string start = "";
	std::string end = "";
	int limit = 2147483647;
	this->iter = backend->ssdb->iterator(start, end, limit);
	this->status = Client::DUMP;
	this->next_seq = 0;
	this->last_key = "";
}

void BackendSync::Client::dump(){
	Buffer *output = link->output;
	if(!this->iter->next()){
		delete this->iter;
		this->iter = NULL;
		this->status = Client::SYNC;
		log_info("fd: %d, dump end", link->fd());

		output->append_record("dump_end");
		output->append('\n');
	}else{
		Bytes key = this->iter->key();
		Bytes val = this->iter->val();
		this->last_key = key.String();
		log_trace("fd: %d, dump: %s", link->fd(), hexmem(key.data(), key.size()).c_str());

		output->append_record("sync_set");
		output->append_record("0");
		output->append_record(key);
		output->append_record(val);
		output->append('\n');
	}
}

int BackendSync::Client::sync(SyncLogQueue *logs){
	Buffer *output = link->output;

	SyncLog log;
	int ret;
	if(this->status == Client::DUMP && this->next_seq == 0){
		ret = logs->find_last(&log);
	}else{
		ret = logs->find(this->next_seq, &log);
	}
	if(ret == 0){
		return 0;
	}

	uint64_t expect_seq = this->next_seq;
	this->next_seq = log.seq() + 1;

	if(this->iter && log.key() > this->last_key){
		log_trace("fd: %d, seq: %llu, drop: %s, last_key: %s",
			link->fd(),
			log.seq(),
			hexmem(log.key().data(), log.key().size()).c_str(),
			hexmem(this->last_key.data(), this->last_key.size()).c_str());
		return 0;
	}

	if(expect_seq != 0 && log.seq() != expect_seq){
		log_warn("fd: %d, OUT_OF_SYNC! seq: %llu, next_seq: %llu",
			link->fd(),
			log.seq(),
			expect_seq
			);
		this->status = Client::OUT_OF_SYNC;
	}else{
		char buf[20];
		snprintf(buf, sizeof(buf), "%llu", log.seq());
		if(log.type() == SyncLog::SET){
			std::string val;
			int ret = backend->ssdb->raw_get(log.key(), &val);
			if(ret == -1){
				log_error("raw_get error!");
			}else if(ret == 0){
				log_trace("skip not found: %s", hexmem(log.key().data(), log.key().size()).c_str());
				// not found, ignore
			}else{
				log_trace("fd: %d, sync: set %llu %s",
					link->fd(),
					log.seq(),
					hexmem(log.key().data(), log.key().size()).c_str());

				output->append_record("sync_set");
				output->append_record(buf);
				output->append_record(log.key());
				output->append_record(val);
				output->append('\n');
			}
		}else if(log.type() == SyncLog::DEL){
				log_trace("fd: %d, sync: del %llu %s",
					link->fd(),
					log.seq(),
					hexmem(log.key().data(), log.key().size()).c_str());

			output->append_record("sync_del");
			output->append_record(buf);
			output->append_record(log.key());
			output->append('\n');
		}else{
			log_error("unknown sync log type: %d", log.type());
		}
		return 1;
	}
	return 0;
}
