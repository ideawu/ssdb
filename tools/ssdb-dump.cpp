/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "include.h"
#include <sys/types.h>
#include <sys/stat.h>

#include <string>
#include <vector>

#include "leveldb/db.h"
#include "leveldb/options.h"
#include "leveldb/slice.h"
#include "leveldb/iterator.h"

#include "include.h"
#include "ssdb/const.h"
#include "net/link.h"
#include "util/log.h"
#include "util/file.h"
#include "util/strings.h"

struct Config {
	std::string ip;
	int port;
	bool hasauth;
	std::string auth;
	std::string output_folder;
};

template<class T>
static std::string serialize_req(T &req){
	std::string ret;
	char buf[50];
	for(int i=0; i<req.size(); i++){
		if(i >= 5 && i < req.size() - 1){
			sprintf(buf, "[%d more...]", (int)req.size() - i - 1);
			ret.append(buf);
			break;
		}
		if(((req[0] == "get" || req[0] == "set") && i == 1) || req[i].size() < 30){
			std::string h = hexmem(req[i].data(), req[i].size());
			ret.append(h);
		}else{
			sprintf(buf, "[%d bytes]", (int)req[i].size());
			ret.append(buf);
		}
		if(i < req.size() - 1){
			ret.append(" ");
		}
	}
	return ret;
}

void welcome(){
	printf("ssdb-dump - SSDB backup command\n");
	printf("Copyright (c) 2012-2015 ssdb.io\n");
	printf("\n");
}

void usage(int argc, char **argv){
	printf("Usage:\n"
		"\n"
		"    %s -o output_folder\n"
		"    %s ip port output_folder\n"
		"\n"
		"Options:\n"
		"    -h <ip/hostname>   Server IP address/hostname (default: 127.0.0.1).\n"
		"    -p <port>          Server port (default: 8888).\n"
		"    -a <password>      Password to use when connecting to the server.\n"
		"    -o <output_folder> local backup folder that will be created.\n"
		"\n",
		argv[0], argv[0]);
	exit(1);   
}

int parse_options(Config *config, int argc, char **argv){
	int i;
	for(i = 1; i < argc; i++) {
		bool lastarg = i==argc-1;
		if(!strcmp(argv[i],"-h") && !lastarg){
			config->ip = argv[++i];
		}else if(!strcmp(argv[i], "-h") && lastarg){
			usage(argc, argv);
		}else if(!strcmp(argv[i], "-p") && !lastarg){
			config->port = atoi(argv[++i]);
		}else if(!strcmp(argv[i], "-a") && !lastarg){
			config->hasauth = true;
			config->auth = argv[++i];
		}else if(!strcmp(argv[i], "-o") && !lastarg){
			config->output_folder = argv[++i];
		}else{
			if(argv[i][0] == '-'){
				fprintf(stderr,
					"Unrecognized option or bad number of args for: '%s'\n",
					argv[i]);
					exit(1);
			}else{
				/* Likely the command name, stop here. */
				break;
			}
		}
	}
	return i;
}

int main(int argc, char **argv){
	welcome();
	set_log_level(Logger::LEVEL_MIN);

	Config config;
	config.ip = "127.0.0.1";
	config.port = 8888;
	config.hasauth = false;
    
	int firstarg = parse_options(&config, argc, argv);
	if(firstarg == 1 && firstarg + 3 <= argc){
		// compatibale with old style arguments
		config.ip = argv[firstarg + 0];
		config.port = atoi(argv[firstarg + 1]);
		config.output_folder = argv[firstarg + 2];
	}

	if(config.output_folder.empty()){
		fprintf(stderr, "ERROR: -o <output_folder> is required!\n");
		usage(argc, argv);
		exit(1);
	}
    
	if(file_exists(config.output_folder.c_str())){
		fprintf(stderr, "ERROR: output_folder[%s] exists!\n", config.output_folder.c_str());
		exit(1);
	}
	if(mkdir(config.output_folder.c_str(), 0777) == -1){
		fprintf(stderr, "ERROR: error create backup directory!\n");
		exit(1);
	}

	std::string data_dir = "";
	data_dir.append(config.output_folder);
	data_dir.append("/data");
	
	{
		std::string meta_dir = "";
		meta_dir.append(config.output_folder);
		meta_dir.append("/meta");

		int ret;
		ret = mkdir(meta_dir.c_str(), 0755);
		if(ret == -1){
			fprintf(stderr, "ERROR: error creating meta dir\n");
			exit(1);
		}
	}

	// connect to server
	Link *link = Link::connect(config.ip.c_str(), config.port);
	if(link == NULL){
		fprintf(stderr, "ERROR: error connecting to server: %s:%d!\n", config.ip.c_str(), config.port);
		exit(1);
	}
	if(config.hasauth){
		const std::vector<Bytes> *resp = link->request("auth", config.auth.c_str());
		if(resp == NULL || resp->at(0) != "ok"){
			fprintf(stderr, "ERROR: auth error!\n");
			exit(1);
		}
	}
	link->send("dump", "A", "", "-1");
	link->flush();

	leveldb::DB* db;
	leveldb::Options options;
	leveldb::Status status;
	options.create_if_missing = true;
	options.write_buffer_size = 32 * 1024 * 1024;
	options.compression = leveldb::kSnappyCompression;

	status = leveldb::DB::Open(options, data_dir.c_str(), &db);
	if(!status.ok()){
		fprintf(stderr, "ERROR: open leveldb: %s error!\n", config.output_folder.c_str());
		exit(1);
	}

	int64_t dump_count = 0;
	while(1){
		const std::vector<Bytes> *req = link->recv();
		if(req == NULL){
			fprintf(stderr, "recv error\n");
			fprintf(stderr, "ERROR: failed to dump data!\n");
			exit(1);
		}else if(req->empty()){
			int len = link->read();
			if(len <= 0){
				fprintf(stderr, "read error: %s\n", strerror(errno));
				fprintf(stderr, "ERROR: failed to dump data!\n");
				exit(1);
			}
		}else{
			Bytes cmd = req->at(0);
			if(cmd == "begin"){
				printf("recv begin...\n");
			}else if(cmd == "end"){
				printf("received %" PRId64 " entry(s)\n", dump_count);
				printf("recv end\n\n");
				break;
			}else if(cmd == "set"){
				/*
				std::string s = serialize_req(*req);
				printf("%s\n", s.c_str());
				*/

				if(req->size() != 3){
					fprintf(stderr, "invalid set params!\n");
					fprintf(stderr, "ERROR: failed to dump data!\n");
					exit(1);
				}
				Bytes key = req->at(1);
				Bytes val = req->at(2);
				if(key.size() == 0 || key.data()[0] == DataType::SYNCLOG){
					continue;
				}
				
				leveldb::Slice k(key.data(), key.size());
				leveldb::Slice v(val.data(), val.size());
				status = db->Put(leveldb::WriteOptions(), k, v);
				//printf("set %s %s\n", str_escape(key.data(), key.size()).c_str(), str_escape(val.data(), val.size()).c_str());
				if(!status.ok()){
					fprintf(stderr, "put leveldb error!\n");
					fprintf(stderr, "ERROR: failed to dump data!\n");
					exit(1);
				}

				dump_count ++;
				if((int)log10(dump_count - 1) != (int)log10(dump_count) || (dump_count > 0 && dump_count % 100000 == 0)){
					printf("received %" PRId64 " entry(s)\n", dump_count);
				}
			}else{
				fprintf(stderr, "error: unknown command %s\n", std::string(cmd.data(), cmd.size()).c_str());
				fprintf(stderr, "ERROR: failed to dump data!\n");
				exit(1);
			}
		}
	}
	printf("total dumped %" PRId64 " entry(s)\n", dump_count);

	{
		std::string val;
		if(db->GetProperty("leveldb.stats", &val)){
			printf("%s\n", val.c_str());
		}
	}

	printf("compacting data...\n");
	db->CompactRange(NULL, NULL);
	
	{
		std::string val;
		if(db->GetProperty("leveldb.stats", &val)){
			printf("%s\n", val.c_str());
		}
	}

	printf("backup has been made to folder: %s\n", config.output_folder.c_str());
	
	delete link;
	delete db;
	return 0;
}
