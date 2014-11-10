/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef NET_SERVER_H_
#define NET_SERVER_H_

#include "../include.h"
#include <string>
#include <vector>

#include "fde.h"
#include "proc.h"
#include "worker.h"

class Link;
class Config;
class IpFilter;
class Fdevents;

typedef std::vector<Link *> ready_list_t;

class NetworkServer
{
private:
	int tick_interval;
	int status_report_ticks;

	Config *conf;
	Link *serv_link;
	IpFilter *ip_filter;
	Fdevents *fdes;

	Link* accept_link();
	int proc_result(ProcJob *job, ready_list_t *ready_list);
	int proc_client_event(const Fdevent *fde, ready_list_t *ready_list);

	void proc(ProcJob *job);

	static const int READER_THREADS = 10;
	static const int WRITER_THREADS = 1;
	ProcWorkerPool *writer;
	ProcWorkerPool *reader;

protected:
	void usage(int argc, char **argv);

public:
	void *data;
	ProcMap proc_map;
	int link_count;
	bool need_auth;
	std::string password;

	NetworkServer();
	~NetworkServer();
	
	void init(const char *conf_file);
	void init(const Config &conf);
	void serve();
};


#endif
