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

#define PROC(c, f)     net->proc_map.set_proc(#c, f, proc_##c)

void ClusterServer::reg_procs(NetworkServer *net){
	PROC(info, "w");
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
	resp->push_back("hello world!");
	return 0;
}
