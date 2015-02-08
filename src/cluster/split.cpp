#include "split.h"
#include <stdlib.h>
#include <vector>
#include "ssdb_client.h"
#include "../util/log.h"
#include "../util/strings.h"

static const int COPY_BATCH_SIZE = 2;

Split::Split(){
	cluster = NULL;
	src_client = NULL;
	dst_client = NULL;
}

Split::~Split(){
	delete cluster;
	delete src_client;
	delete dst_client;
}
	
int Split::init(const std::string &cluster_ip, int cluster_port, const std::string &src_ip, int src_port, const std::string &dst_ip, int dst_port){
	{
		char buf[128];
		snprintf(buf, sizeof(buf), "split|%s:%d|%s:%d", src_ip.c_str(), src_port, dst_ip.c_str(), dst_port);
		this->status_key = buf;
	}

	cluster = ssdb::Client::connect(cluster_ip.c_str(), cluster_port);
	if(cluster == NULL){
		log_error("fail to connect to cluster server!");
		return -1;
	}

	src_client = ssdb::Client::connect(src_ip.c_str(), src_port);
	if(src_client == NULL){
		log_error("fail to connect to src server!");
		return -1;
	}

	dst_client = ssdb::Client::connect(dst_ip.c_str(), dst_port);
	if(dst_client == NULL){
		log_error("fail to connect to dst server!");
		return -1;
	}
	
	ssdb::Status s;
	s = cluster->hget(status_key, "src_key", &last_move_key);
	if(!s.ok()){
		return -1;
	}
	if(!last_move_key.empty()){
		log_debug("recover, last_move_key: \"%s\"", str_escape(last_move_key).c_str());
		last_move_key = ""; // reset last_key, migrate from start
	}

	return 0;
}

int Split::finish(){
	ssdb::Status s;
	s = cluster->hclear(status_key);
	if(!s.ok()){
		return -1;
	}
	return 0;
}

int64_t Split::move_some(){
	int ret;
	std::string min_key;
	std::string max_key;
	ret = find_src_key_range_to_move(&min_key, &max_key);
	if(ret == -1){
		return -1;
	}
	if(ret == 0){
		return 0;
	}
	int64_t size = move_key_range(min_key, max_key);
	return size;
}

int Split::find_src_key_range_to_move(std::string *min_key, std::string *max_key){
	std::vector<std::string> keys;
	ssdb::Status s;
	s = src_client->keys(last_move_key, "", COPY_BATCH_SIZE, &keys);
	if(!s.ok()){
		log_error("response error: %s", s.code().c_str());
		return -1;
	}
	if(keys.empty()){
		return 0;
	}
	*min_key = last_move_key;
	*max_key = keys[keys.size() - 1];
	return 1;
}
	
int64_t Split::move_key_range(const std::string &min_key, const std::string &max_key){
	int64_t size = 0;
	
	set_src_key(max_key);
	
	ssdb::Status s;
	std::string iterate_key_start = min_key;
	while(1){
		std::vector<std::string> keys;
		ssdb::Status s;
		s = src_client->keys(iterate_key_start, max_key, COPY_BATCH_SIZE, &keys);
		if(!s.ok()){
			log_error("response error: %s", s.code().c_str());
			return -1;
		}
		if(keys.empty()){
			break;
		}
		
		// move keys one by one, because scan() may exceed max_packet_size
		for(int i=0; i<keys.size(); i++){
			int ret = move_key(keys[i]);
			if(ret == -1){
				return -1;
			}
			size += ret;
		}
		
		iterate_key_start = keys[keys.size() - 1];
		if(iterate_key_start == max_key){
			break;
		}
	}
	return size;
}

int Split::move_key(const std::string &key){
	log_debug("%s \"%s\"", __FUNCTION__, str_escape(key).c_str());

	int size = 0;
	int res;
	
	res = copy_key(key);
	if(res == -1){
		return -1;
	}
	size += res;
	
	res = log_src_key(key);
	if(res == -1){
		return -1;
	}

	res = set_dst_key(key);
	if(res == -1){
		return -1;
	}

	res = log_dst_key(key);
	if(res == -1){
		return -1;
	}

	res = del_src_key(key);
	if(res == -1){
		return -1;
	}
	
	last_move_key = key;
	return size;
}

int Split::log_src_key(const std::string &key){
	log_debug("%s \"%s\"", __FUNCTION__, str_escape(key).c_str());
	ssdb::Status s;
	s = cluster->hset(this->status_key, "src_key", key);
	if(!s.ok()){
		log_error("write status error!");
		return -1;
	}
	return 0;
}
	
int Split::log_dst_key(const std::string &key){
	log_debug("%s \"%s\"", __FUNCTION__, str_escape(key).c_str());
	ssdb::Status s;
	s = cluster->hset(this->status_key, "dst_key", key);
	if(!s.ok()){
		log_error("write status error!");
		return -1;
	}
	return 0;
}

int Split::copy_key(const std::string &key){
	log_debug("%s \"%s\"", __FUNCTION__, str_escape(key).c_str());
	std::string val;
	ssdb::Status s;
	s = src_client->get(key, &val);
	if(s.not_found()){
		return 0;
	}
	if(!s.ok()){
		log_error("read from src error: %s", s.code().c_str());
		return -1;
	}
	log_debug("copy %s = %s", key.c_str(), val.c_str());
			
	s = dst_client->set(key, val);
	if(!s.ok()){
		log_error("write to dst error: %s", s.code().c_str());
		return -1;
	}
	return (int)(key.size() + val.size() + 2);
}

int Split::set_src_key(const std::string &key){
	log_debug("%s \"%s\"", __FUNCTION__, str_escape(key).c_str());
	// TODO:
	return 0;
}

int Split::set_dst_key(const std::string &key){
	log_debug("%s \"%s\"", __FUNCTION__, str_escape(key).c_str());
	// TODO:
	return 0;
}

int Split::del_src_key(const std::string &key){
	log_debug("%s \"%s\"", __FUNCTION__, str_escape(key).c_str());
	ssdb::Status s;
	s = src_client->del(key);
	if(!s.ok()){
		log_error("delete src key error: %s", s.code().c_str());
		return -1;
	}
	return 0;
}

