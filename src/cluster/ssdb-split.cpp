/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "../util/log.h"
#include "../util/strings.h"
#include "SSDB.h"

void welcome(){
	printf("ssdb-split - Split ssdb-server\n");
	printf("Copyright (c) 2012-2015 ssdb.io\n");
	printf("\n");
}

void usage(int argc, char **argv){
	printf("Usage:\n");
	printf("    %sr\n", argv[0]);
	printf("\n");
	printf("Options:\n");
	printf("    xx - \n");
}

ssdb::Client *src_client;
ssdb::Client *dst_client;
const int COPY_BATCH_SIZE = 2;

int copy_kv_data(const std::string &start_key, const std::string &end_key);


int main(int argc, char **argv){
	welcome();
	
	const char *src_ip = "127.0.0.1";
	int src_port = 8888;
	const char *dst_ip = "127.0.0.1";
	int dst_port = 8889;

	src_client = ssdb::Client::connect(src_ip, src_port);
	if(src_client == NULL){
		printf("fail to connect to src server!\n");
		return 0;
	}

	dst_client = ssdb::Client::connect(dst_ip, dst_port);
	if(dst_client == NULL){
		printf("fail to connect to dst server!\n");
		return 0;
	}
	
	std::string start_key;
	while(1){
		std::vector<std::string> keys;
		ssdb::Status s;
		s = src_client->keys(start_key, "", COPY_BATCH_SIZE, &keys);
		if(!s.ok()){
			log_error("response error: %s", s.code().c_str());
			break;
		}
		if(keys.empty()){
			break;
		}
		
		std::string end_key = keys[keys.size() - 1];
		
		// 1. lock key range in ("", end_key]
		log_debug("lock (\"\", \"%s\"] for read", str_escape(end_key).c_str());
		
		// 2. copy keys in range (start_key, end_key]
		log_debug("copy (\"%s\", \"%s\"] begin", str_escape(start_key).c_str(), str_escape(end_key).c_str());
		int ret = copy_kv_data(start_key, end_key);
		if(ret == -1){
			log_error("copy error!");
			break;
		}
		log_debug("copy (\"%s\", \"%s\"] end", str_escape(start_key).c_str(), str_escape(end_key).c_str());
		
		start_key = end_key;
	}


	delete src_client;
	delete dst_client;
	return 0;
}

int copy_kv_data(const std::string &start_key, const std::string &end_key){
	int ret = 0;
	ssdb::Status s;
	std::string iterate_key_start = start_key;
	while(1){
		std::vector<std::string> keys;
		ssdb::Status s;
		s = src_client->keys(iterate_key_start, end_key, COPY_BATCH_SIZE + 100, &keys);
		if(!s.ok()){
			log_error("response error: %s", s.code().c_str());
			return -1;
		}
		if(keys.empty()){
			break;
		}
		ret += (int)keys.size();
		
		// copy keys one by one, because scan() may exceed max_packet_size
		for(int i=0; i<keys.size(); i++){
			const std::string &key = keys[i];
			std::string val;
			s = src_client->get(key, &val);
			if(!s.ok()){
				log_error("read from src error: %s", s.code().c_str());
				return -1;
			}
			
			//log_debug("copy %s = %s", key.c_str(), val.c_str());
			
			s = dst_client->set(key, val);
			if(!s.ok()){
				log_error("write to dst error: %s", s.code().c_str());
				return -1;
			}
		}
		
		iterate_key_start = keys[keys.size() - 1];
		if(iterate_key_start == end_key){
			break;
		}
	}
	return ret;
}
