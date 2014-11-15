/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "server.h"

void usage(int argc, char **argv){
	printf("Usage:\n");
	printf("    %s [-d] /path/to/server.conf\n", argv[0]);
	printf("Options:\n");
	printf("    -d    run as daemon\n");
}

DEF_PROC(hello);

int main(int argc, char **argv){
	bool is_daemon = false;
	const char *conf_file = NULL;
	for(int i=1; i<argc; i++){
		if(strcmp(argv[i], "-d") == 0){
			is_daemon = true;
		}else{
			conf_file = argv[i];
		}
	}
	if(conf_file == NULL){
		usage(argc, argv);
		exit(1);
	}

	NetworkServer *serv = NetworkServer::init(conf_file);
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
