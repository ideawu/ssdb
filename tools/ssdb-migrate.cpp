#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "util/log.h"
#include "util/strings.h"
#include "SSDB_client.h"

ssdb::Client *src = NULL;
ssdb::Client *dst = NULL;
int batch_size = 10;

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
		log_debug("lock range %s", KeyRange(min_key, *moved_max_key).str().c_str());
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
			exit(0);
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

int main(int argc, char **argv){
	const char *src_ip = "127.0.0.1";
	int src_port = 8887;
	const char *dst_ip = "127.0.0.1";
	int dst_port = 8889;
	int limit = 13; // aproximated number of keys to be deleted

	src = init_client(src_ip, src_port);
	if(src == NULL){
		log_error("fail to connect to server!");
		return 0;
	}
	dst = init_client(dst_ip, dst_port);
	if(dst == NULL){
		log_error("fail to connect to server!");
		return 0;
	}
	
	KeyRange src_range;
	if(get_key_range(src, &src_range) == -1){
		return -1;
	}
	log_debug("old src %s", src_range.str().c_str());
	
	KeyRange dst_range;
	if(get_key_range(dst, &dst_range) == -1){
		return -1;
	}
	log_debug("old dst %s", dst_range.str().c_str());

	for(int i=0; i<limit; i+=batch_size){
		int num = batch_size;
		if(limit - i < batch_size){
			num = limit - i;
		}
		
		// move data
		int ret;
		std::string moved_max_key;
		ret = move_range(src_range.start, src_range.end, num, &moved_max_key);
		if(ret == -1){
			log_fatal("move_range error!");
			exit(0);
		}
		while(ret == num){
			// check again, make sure there is not key inserted before we lock range
			ret = move_range(src_range.start, moved_max_key, num, NULL);
			if(ret == -1){
				log_fatal("move_range error!");
				exit(0);
			}
		}
	
		KeyRange new_src_range(moved_max_key, src_range.end);
		KeyRange new_dst_range(dst_range.start, moved_max_key);
	
		log_debug("new src %s", new_src_range.str().c_str());
		log_debug("new dst %s", new_dst_range.str().c_str());
	
		// update key range
		if(set_key_range(src, new_src_range) == -1){
			log_error("src server set_kv_range error!");
			return -1;
		}
		if(set_key_range(dst, new_dst_range) == -1){
			log_error("dst server set_kv_range error!");
			return -1;
		}
	}
	
	delete src;
	delete dst;
	return 0;
}
