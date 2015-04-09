/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "include.h"

#include <string>
#include <vector>

#include "leveldb/db.h"
#include "leveldb/options.h"
#include "leveldb/slice.h"
#include "leveldb/iterator.h"

#include "net/link.h"
#include "util/log.h"
#include "util/file.h"
#include "util/strings.h"

void welcome(){
	printf("leveldb-import - Import existing leveldb into ssdb\n");
	printf("Copyright (c) 2013-2015 ssdb.io\n");
	printf("\n");
}

void usage(int argc, char **argv){
	printf("Usage:\n");
	printf("    %s ip port input_folder\n", argv[0]);
	printf("\n");
	printf("Options:\n");
	printf("    ip - ssdb server ip address\n");
	printf("    port - ssdb server port number\n");
	printf("    input_folder - local leveldb folder\n");
}

int main(int argc, char **argv){
	welcome();

	set_log_level(Logger::LEVEL_MIN);

	if(argc <= 3){
		usage(argc, argv);
		return 0;
	}
	char *ip = argv[1];
	int port = atoi(argv[2]);
	char *input_folder = argv[3];

	if(!file_exists(input_folder)){
		printf("input_folder[%s] not exists!\n", input_folder);
		return 0;
	}

	std::string data_dir = "";
	data_dir.append(input_folder);

	// connect to server
	Link *link = Link::connect(ip, port);
	if(link == NULL){
		printf("error connecting to server!\n");
		return 0;
	}

	leveldb::DB* db;
	leveldb::Options options;
	leveldb::Status status;
	//options.create_if_missing = true;
	status = leveldb::DB::Open(options, data_dir.c_str(), &db);
	if(!status.ok()){
		printf("open leveldb: %s error!\n", input_folder);
		return 0;
	}

	printf("importing data...\n");
	leveldb::Iterator *it;
	it = db->NewIterator(leveldb::ReadOptions());
	int save_count = 0;
	for(it->SeekToFirst(); it->Valid(); it->Next()){
		std::string key = it->key().ToString();
		std::string val = it->value().ToString();
		
		const std::vector<Bytes> *req = link->request("set", key, val);
		if(req == NULL){
			printf("error\n");
			exit(0);
		}else{
			if(req->at(0) != "ok"){
				printf("server response error: %s\n", req->at(0).String().c_str());
				exit(0);
			}
		}
		save_count ++;
	}
	printf("importing done.\n");
	printf("\n");
	printf("total %d item(s) imported.\n", save_count);

	delete link;
	delete db;
	return 0;
}
