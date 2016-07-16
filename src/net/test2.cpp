/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "server.h"
#include "../util/config.h"

DEF_PROC(hello);

int main(int argc, char **argv){
	fprintf(stderr, "Usage: %s [ip] [port]\n\n", argv[0]);
	Config conf;
	conf.set("server.port", "9000");
	if(argc > 1){
		conf.set("server.ip", argv[1]);
	}
	if(argc > 2){
		conf.set("server.port", argv[2]);
	}
	NetworkServer *serv = NetworkServer::init(conf);
	if(!serv){
		exit(1);
	}
	// register command procedure
	serv->proc_map.set_proc("hello", proc_hello);
	serv->serve();
	delete serv;
	return 0;
}

int proc_hello(NetworkServer *net, Link *link, const Request &req, Response *resp){
	resp->push_back("ok");
	resp->push_back("world!");
	if(req.size() > 1){
		// The first argument start at index 1, just like argv.
		resp->push_back(req[1].String());
	}
	return 0;
}
