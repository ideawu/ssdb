/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "version.h"
#include "../util/log.h"
#include "../util/strings.h"
#include "../net/proc.h"
#include "../net/server.h"
#include "serv.h"

DEF_PROC(info);
DEF_PROC(dbsize);

#define PROC(c, f)     net->proc_map.set_proc(#c, f, proc_##c)

void ClusterServer::reg_procs(NetworkServer *net){
	PROC(info, "w");
	PROC(dbsize, "w");
}

ClusterServer::ClusterServer(SSDB *ssdb, const Config &conf, NetworkServer *net){
	this->ssdb = (SSDBImpl *)ssdb;

	net->data = this;
	this->reg_procs(net);
}

ClusterServer::~ClusterServer(){
	log_debug("ClusterServer finalized");
}

int proc_info(NetworkServer *net, Link *link, const Request &req, Response *resp){
	ClusterServer *serv = (ClusterServer *)net->data;
	resp->push_back("ok");
	resp->push_back("ssdb-cluster");
	resp->push_back("version");
	resp->push_back(SSDB_CLUSTER_VERSION);
	{
		resp->push_back("links");
		resp->add(net->link_count);
	}
	{
		int64_t calls = 0;
		proc_map_t::iterator it;
		for(it=net->proc_map.begin(); it!=net->proc_map.end(); it++){
			Command *cmd = it->second;
			calls += cmd->calls;
		}
		resp->push_back("total_calls");
		resp->add(calls);
	}
	
	{
		uint64_t size = serv->ssdb->size();
		resp->push_back("dbsize");
		resp->push_back(str(size));
	}

	{
		std::string s = serv->ssdb->binlogs->stats();
		resp->push_back("binlogs");
		resp->push_back(s);
	}

	return 0;
}

int proc_dbsize(NetworkServer *net, Link *link, const Request &req, Response *resp){
	ClusterServer *serv = (ClusterServer *)net->data;
	uint64_t size = serv->ssdb->size();
	resp->push_back("ok");
	resp->push_back(str(size));
	return 0;
}
