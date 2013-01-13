#include <pthread.h>
#include "backend_dump.h"

BackendDump::BackendDump(const SSDB *ssdb){
	this->ssdb = ssdb;
}

BackendDump::~BackendDump(){
	log_debug("BackendDump finalized");
}

void BackendDump::proc(const Link *link){
	log_info("accept dump client");
	struct run_arg *arg = new run_arg();
	arg->link = link;
	arg->backend = this;

	pthread_t tid;
	int err = pthread_create(&tid, NULL, &BackendDump::_run_thread, arg);
	if(err != 0){
		log_error("can't create thread: %s", strerror(err));
		delete link;
	}
}

void* BackendDump::_run_thread(void *arg){
	struct run_arg *p = (struct run_arg*)arg;
	const BackendDump *backend = p->backend;
	Link *link = (Link *)p->link;
	delete p;

	//
	link->noblock(false);

	const std::vector<Bytes>* req = link->last_recv();

	std::string start = "";
	if(req->size() > 1){
		Bytes b = req->at(1);
		start.assign(b.data(), b.size());
	}
	std::string end = "";
	if(req->size() > 2){
		Bytes b = req->at(2);
		end.assign(b.data(), b.size());
	}
	int limit = 10;
	if(req->size() > 3){
		Bytes b = req->at(3);
		limit = b.Int();
	}

	Buffer *output = link->output;

	int count = 0;
	bool quit = false;
	Iterator *it = backend->ssdb->iterator(start, end, limit);

	link->send("begin");
	while(!quit){
		if(!it->next()){
			quit = true;
			char buf[20];
			snprintf(buf, sizeof(buf), "%d", count);
			link->send("end", buf);
		}else{
			count ++;
			Bytes key = it->key();
			Bytes val = it->val();

			output->append_record("set");
			output->append_record(key);
			output->append_record(val);
			output->append('\n');

			if(output->size() < output->total()/2){
				continue;
			}
		}

		if(link->flush() == -1){
			log_info("fd: %d, send error", link->fd());
			break;
		}
	}

	log_info("fd: %d, delete link", link->fd());
	delete link;
	return (void *)NULL;
}
