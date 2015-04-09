#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "util/log.h"
#include "util/strings.h"
#include "SSDB_client.h"

#define BATCH_SIZE 100

ssdb::Client *src = NULL;
ssdb::Client *dst = NULL;

void welcome(){
	printf("ssdb-migrate - SSDB server migration tool\n");
	printf("Copyright (c) 2012-2015 ssdb.io\n");
	printf("\n");
}

void usage(int argc, char **argv){
	printf("Usage:\n"
		"    %s type src_ip src_port dst_ip dst_port limit\n"
		"\n"
		"Options:\n"
		"    type      Supported values: KV\n"
		"    src_ip    IP addr of the SSDB server to move data from, example: 127.0.0.1\n"
		"    src_port  Port number of source SSDB server\n"
		"    src_ip    IP addr of the SSDB server to move data to, example: 127.0.0.1\n"
		"    dst_port  Port number of destination SSDB server\n"
		"    limit     Approximated number of keys to be moved, example: 1000\n"
		"    -h        Show this message"
		"\n"
		"Example:\n"
		"    %s KV 127.0.0.1 8887 127.0.0.1 8889 13\n"
		"\n",
		argv[0], argv[0]);
	exit(1);   
}

struct AppArgs{
	std::string type;
	std::string src_ip;
	int src_port;
	std::string dst_ip;
	int dst_port;
	int limit;
};

void parse_args(AppArgs *args, int argc, char **argv){
	if(argc < 7){
		usage(argc, argv);
	}
	for(int i=1; i<argc; i++){
		if(std::string("-h") == argv[i]){
			usage(argc, argv);
		}
		if(argv[i][0] == '-'){
			fprintf(stderr, "ERROR: Invalid argument: %s!\n", argv[i]);
			exit(1);   
		}
	}
	args->type = argv[1];
	args->src_ip = argv[2];
	args->src_port = str_to_int(argv[3]);
	args->dst_ip = argv[4];
	args->dst_port = str_to_int(argv[5]);
	args->limit = str_to_int(argv[6]);
	if(args->type != "KV"){
		fprintf(stderr, "ERROR: only type of KV is supported!\n");
		exit(1);   
	}
	if(args->limit <= 0){
		fprintf(stderr, "ERROR: invalid limit option!\n");
		exit(1);   
	}
}

struct KeyRange{
	std::string start;
	std::string end;
	
	KeyRange(){
	}
	
	KeyRange(const std::string &start, const std::string &end){
		this->start = start;
		this->end = end;
	}
	
	std::string str(){
		char buf[1024];
		snprintf(buf, sizeof(buf), "(\"%s\", \"%s\"]", str_escape(start).c_str(), str_escape(end).c_str());
		return std::string(buf);
	}
};

int move_key(const std::string &key){
	std::string val;
	ssdb::Status s;
	s = src->get(key, &val);
	if(s.not_found()){
		return 0;
	}
	if(!s.ok()){
		log_error("src server error! %s", s.code().c_str());
		return -1;
	}
	s = dst->set(key, val);
	if(!s.ok()){
		log_error("dst server error! %s", s.code().c_str());
		return -1;
	}
	s = src->del(key);
	if(!s.ok()){
		log_error("src server error! %s", s.code().c_str());
		return -1;
	}
	return 1;
}

int move_range(const std::string &min_key, const std::string &max_key, int limit, std::string *moved_max_key){
	// get key range
	std::vector<std::string> keys;
	ssdb::Status s;
	s = src->keys(min_key, max_key, limit, &keys);
	if(!s.ok()){
		log_error("response error: %s", s.code().c_str());
		return -1;
	}
	if(keys.empty()){
		return 0;
	}
	if(moved_max_key){
		*moved_max_key = keys[keys.size() - 1];

		// lock key range
		log_info("lock range %s", KeyRange(min_key, *moved_max_key).str().c_str());
		const std::vector<std::string>* resp;
		resp = src->request("set_kv_range", *moved_max_key, max_key);
		if(!resp || resp->empty() || resp->at(0) != "ok"){
			log_error("src server set_kv_range error!");
			return -1;
		}
	}

	// move key range
	for(int i=0; i<(int)keys.size(); i++){
		const std::string &key = keys[i];
		if(move_key(key) == -1){
			log_fatal("move key %s error! %s", key.c_str(), s.code().c_str());
			exit(1);   
		}
	}
	
	return (int)keys.size();
}

ssdb::Client* init_client(const std::string &ip, int port){
	ssdb::Client *client = ssdb::Client::connect(ip, port);
	if(client == NULL){
		log_error("fail to connect to server!");
		return NULL;
	}

	const std::vector<std::string>* resp;
	resp = client->request("ignore_key_range");
	if(!resp || resp->empty() || resp->at(0) != "ok"){
		log_error("src server ignore_key_range error!");
		delete client;
		return NULL;
	}
	return client;
}

int get_key_range(ssdb::Client *client, KeyRange *range){
	const std::vector<std::string>* resp;
	resp = client->request("get_kv_range");
	if(!resp || resp->size() < 3 || resp->at(0) != "ok"){
		log_error("get_kv_range error!");
		return -1;
	}
	range->start = resp->at(1);
	range->end = resp->at(2);
	return 0;
}

int set_key_range(ssdb::Client *client, const KeyRange &range){
	const std::vector<std::string>* resp;
	resp = client->request("set_kv_range", range.start, range.end);
	if(!resp || resp->empty() || resp->at(0) != "ok"){
		log_error("server set_kv_range error!");
		return -1;
	}
	return 0;
}

void check_version(ssdb::Client *client){
	const std::vector<std::string>* resp;
	resp = client->request("version");
	if(!resp || resp->size() < 2 || resp->at(0) != "ok"){
		fprintf(stderr, "ERROR: ssdb-server 1.9.0 or higher is required!\n");
		exit(1);
	}
}

int main(int argc, char **argv){
	welcome();
	AppArgs args;
	parse_args(&args, argc, argv);

	src = init_client(args.src_ip, args.src_port);
	if(src == NULL){
		log_error("fail to connect to server!");
		return 0;
	}
	dst = init_client(args.dst_ip, args.dst_port);
	if(dst == NULL){
		log_error("fail to connect to server!");
		return 0;
	}
	check_version(src);
	check_version(dst);
	
	KeyRange src_range;
	if(get_key_range(src, &src_range) == -1){
		return -1;
	}
	log_info("old src %s", src_range.str().c_str());
	
	KeyRange dst_range;
	if(get_key_range(dst, &dst_range) == -1){
		return -1;
	}
	log_info("old dst %s", dst_range.str().c_str());

	for(int i=0; i<args.limit; i+=BATCH_SIZE){
		int num = BATCH_SIZE;
		if(args.limit - i < BATCH_SIZE){
			num = args.limit - i;
		}
		
		// move data
		int ret;
		std::string moved_max_key;
		ret = move_range(src_range.start, src_range.end, num, &moved_max_key);
		if(ret == -1){
			log_fatal("move_range error!");
			exit(1);   
		}
		if(ret == 0){
			continue;
		}
		log_debug("moved %d key(s)", ret);
		while(ret == num){
			// check again, make sure there is not key inserted before we lock range
			ret = move_range(src_range.start, moved_max_key, num, NULL);
			if(ret == -1){
				log_fatal("move_range error!");
				exit(1);   
			}
		}
	
		KeyRange new_src_range(moved_max_key, src_range.end);
		KeyRange new_dst_range(dst_range.start, moved_max_key);
	
		log_info("src %s => %s", src_range.str().c_str(), new_src_range.str().c_str());
		log_info("dst %s => %s", dst_range.str().c_str(), new_dst_range.str().c_str());
	
		// update key range
		if(set_key_range(src, new_src_range) == -1){
			log_fatal("src server set_kv_range error!");
			exit(1);   
		}
		if(set_key_range(dst, new_dst_range) == -1){
			log_fatal("dst server set_kv_range error!");
			exit(1);   
		}
	}
	
	delete src;
	delete dst;
	return 0;
}
