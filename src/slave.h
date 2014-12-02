/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef SSDB_SLAVE_H_
#define SSDB_SLAVE_H_

#include <stdint.h>
#include <string>
#include <pthread.h>
#include <vector>
#include "ssdb/ssdb_impl.h"
#include "ssdb/binlog.h"
#include "net/link.h"

class Slave{
private:
	uint64_t last_seq;
	std::string last_key;
	uint64_t copy_count;
	uint64_t sync_count;
		
	std::string id_;

	SSDB *ssdb;
	SSDB *meta;
	Link *link;
	std::string master_ip;
	int master_port;
	bool is_mirror;
	char log_type;

	static const int DISCONNECTED = 0;
	static const int INIT = 1;
	static const int COPY = 2;
	static const int SYNC = 4;
	int status;

	void migrate_old_status();

	std::string status_key();
	void load_status();
	void save_status();

	volatile bool thread_quit;
	pthread_t run_thread_tid;
	static void* _run_thread(void *arg);
		
	int proc(const std::vector<Bytes> &req);
	int proc_noop(const Binlog &log, const std::vector<Bytes> &req);
	int proc_copy(const Binlog &log, const std::vector<Bytes> &req);
	int proc_sync(const Binlog &log, const std::vector<Bytes> &req);

	unsigned int connect_retry;
	int connect();
	bool connected(){
		return link != NULL;
	}
public:
	std::string auth;
	Slave(SSDB *ssdb, SSDB *meta, const char *ip, int port, bool is_mirror=false);
	~Slave();
	void start();
	void stop();
		
	void set_id(const std::string &id);
	std::string stats() const;
};

#endif
