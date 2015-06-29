/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include <pthread.h>
#include "backend_dump.h"
#include "util/log.h"

BackendDump::BackendDump(SSDB *ssdb){
	this->ssdb = ssdb;
}

BackendDump::~BackendDump(){
	log_debug("BackendDump finalized");
}

void BackendDump::proc(const Link *link){
	log_info("accept dump client: %d", link->fd());
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
	pthread_detach(pthread_self());
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
	if(start.empty()){
		start = "A";
	}
	std::string end = "";
	if(req->size() > 2){
		Bytes b = req->at(2);
		end.assign(b.data(), b.size());
	}
	uint64_t limit = 10;
	if(req->size() > 3){
		Bytes b = req->at(3);
		limit = b.Uint64();
	}

	log_info("fd: %d, begin to dump data: '%s', '%s', %" PRIu64 "",
		link->fd(), start.c_str(), end.c_str(), limit);

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

			if(output->size() < 32 * 1024){
				continue;
			}
		}

		if(link->flush() == -1){
			log_error("fd: %d, send error: %s", link->fd(), strerror(errno));
			break;
		}
	}
	// wait for client to close connection,
	// or client may get a "Connection reset by peer" error.
	link->read();

	log_info("fd: %d, delete link", link->fd());
	delete link;
	delete it;
	return (void *)NULL;
}
