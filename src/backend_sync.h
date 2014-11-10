/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef SSDB_BACKEND_SYNC_H_
#define SSDB_BACKEND_SYNC_H_

#include "include.h"
#include <vector>
#include <string>
#include <map>

#include "ssdb/ssdb_impl.h"
#include "ssdb/binlog.h"
#include "net/link.h"
#include "util/thread.h"

class BackendSync{
private:
	struct Client;
private:
	std::vector<Client *> clients;
	std::vector<Client *> clients_tmp;

	struct run_arg{
		const Link *link;
		const BackendSync *backend;
	};
	volatile bool thread_quit;
	static void* _run_thread(void *arg);
	Mutex mutex;
	std::map<pthread_t, Client *> workers;
	SSDBImpl *ssdb;
	int sync_speed;
public:
	BackendSync(SSDBImpl *ssdb, int sync_speed);
	~BackendSync();
	void proc(const Link *link);
	
	std::vector<std::string> stats();
};

struct BackendSync::Client{
	static const int INIT = 0;
	static const int OUT_OF_SYNC = 1;
	static const int COPY = 2;
	static const int SYNC = 4;

	int status;
	Link *link;
	uint64_t last_seq;
	uint64_t last_noop_seq;
	std::string last_key;
	const BackendSync *backend;
	bool is_mirror;
	
	Iterator *iter;

	Client(const BackendSync *backend);
	~Client();
	void init();
	void reset();
	void noop();
	int copy();
	int sync(BinlogQueue *logs);

	std::string stats();
};

#endif
