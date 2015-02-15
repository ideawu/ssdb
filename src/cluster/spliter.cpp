#include "spliter.h"
#include <stdlib.h>
#include <vector>
#include "../util/log.h"
#include "../util/strings.h"
#include "node.h"

static const int COPY_BATCH_SIZE = 2;

Spliter::Spliter(SSDB *cluster, Node *src_node, Node *dst_node){
	this->cluster = cluster;
	this->src_node = src_node;
	this->dst_node = dst_node;
	this->init();
}

Spliter::~Spliter(){
}
	
int Spliter::init(){
	char buf[128];
	snprintf(buf, sizeof(buf), "split|%d|%d", src_node->id, dst_node->id);
	this->status_key = buf;

	src_kv_range = src_node->kv_range;
	dst_kv_range = dst_node->kv_range;
	
	int ret = load_last_move_key();
	if(ret == -1){
		return -1;
	}
	if(ret == 0){ // not found
		log_info("new split %s", status_key.c_str());
		last_move_key = src_kv_range.start;
	}else{
		log_info("recover split, %s, last_move_key: \"%s\"", status_key.c_str(), str_escape(last_move_key).c_str());
	}
	
	log_info("src_kv_range: %s", src_kv_range.str().c_str());
	log_info("dst_kv_range: %s", dst_kv_range.str().c_str());
	return 0;
}

int Spliter::finish(){
	int64_t ret = cluster->hclear(status_key);
	if(ret == -1){
		log_error("error");
		return -1;
	}
	src_node->kv_range = src_kv_range;
	dst_node->kv_range = dst_kv_range;
	return 0;
}

int Spliter::load_last_move_key(){
	int ret = cluster->hget(this->status_key, "last_move_key", &last_move_key);
	return ret;
}

int Spliter::save_last_move_key(const std::string &key){
	last_move_key = key;
	int ret = cluster->hset(this->status_key, "last_move_key", last_move_key);
	if(ret == -1){
		log_error("write status error!");
		return -1;
	}
	return 0;
}

int64_t Spliter::move_some(){
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

int Spliter::find_src_key_range_to_move(std::string *min_key, std::string *max_key){
	std::vector<std::string> keys;
	ssdb::Status s;
	s = src_node->db->keys(last_move_key, "", COPY_BATCH_SIZE, &keys);
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
	
int64_t Spliter::move_key_range(const std::string &min_key, const std::string &max_key){
	int64_t size = 0;
	
	int res;
	res = save_last_move_key(min_key);
	if(res == -1){
		return -1;
	}
	res = set_src_kv_range(max_key, src_kv_range.end);
	if(res == -1){
		return -1;
	}
	
	ssdb::Status s;
	std::string iterate_key_start = min_key;
	while(1){
		std::vector<std::string> keys;
		ssdb::Status s;
		s = src_node->db->keys(iterate_key_start, max_key, COPY_BATCH_SIZE, &keys);
		if(!s.ok()){
			log_error("response error: %s", s.code().c_str());
			return -1;
		}
		if(keys.empty()){
			break;
		}
		
		// move keys one by one, because scan() may exceed max_packet_size
		for(int i=0; i<keys.size(); i++){
			const std::string &key = keys[i];
			
			res = copy_key(key);
			if(res == -1){
				return -1;
			}
			size += res;
		
			res = del_src_key(key);
			if(res == -1){
				return -1;
			}
		}
		
		iterate_key_start = keys[keys.size() - 1];
		if(iterate_key_start == max_key){
			break;
		}
	}

	res = set_dst_kv_range(dst_kv_range.start, max_key);
	if(res == -1){
		return -1;
	}
	
	res = save_last_move_key(max_key);
	if(res == -1){
		return -1;
	}

	return size;
}

int Spliter::copy_key(const std::string &key){
	//log_debug("%s \"%s\"", __FUNCTION__, str_escape(key).c_str());
	std::string val;
	ssdb::Status s;
	s = src_node->db->get(key, &val);
	if(s.not_found()){
		return 0;
	}
	if(!s.ok()){
		log_error("read from src error: %s", s.code().c_str());
		return -1;
	}
	//log_debug("copy %s = [%d]", key.c_str(), (int)val.size());
			
	s = dst_node->db->set(key, val);
	if(!s.ok()){
		log_error("write to dst error: %s", s.code().c_str());
		return -1;
	}
	return (int)(key.size() + val.size() + 2);
}

int Spliter::set_src_kv_range(const std::string &min_key, const std::string &max_key){
	//log_debug("%s \"%s\"", __FUNCTION__, str_escape(key).c_str());
	src_kv_range.start = min_key;
	src_kv_range.end = max_key;
	
	const std::vector<std::string>* resp;
	resp = src_node->db->request("set_kv_range", min_key, max_key);
	if(!resp || resp->empty() || resp->at(0) != "ok"){
		log_error("src server set_kv_range error!");
		return -1;
	}
	return 0;
}

int Spliter::set_dst_kv_range(const std::string &min_key, const std::string &max_key){
	//log_debug("%s \"%s\"", __FUNCTION__, str_escape(max_key).c_str());
	dst_kv_range.start = min_key;
	dst_kv_range.end = max_key;

	const std::vector<std::string>* resp;
	resp = dst_node->db->request("set_kv_range", min_key, max_key);
	if(!resp || resp->empty() || resp->at(0) != "ok"){
		log_error("dst server set_kv_range error!");
		return -1;
	}
	return 0;
}

int Spliter::del_src_key(const std::string &key){
	//log_debug("%s \"%s\"", __FUNCTION__, str_escape(key).c_str());
	ssdb::Status s;
	s = src_node->db->del(key);
	if(!s.ok()){
		log_error("delete src key error: %s", s.code().c_str());
		return -1;
	}
	return 0;
}

