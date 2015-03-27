#include "cluster_migrate.h"
#include "util/log.h"
#include "SSDB_client.h"

#define BATCH_SIZE 100

static ssdb::Client* init_client(const std::string &ip, int port){
	ssdb::Client *client = ssdb::Client::connect(ip, port);
	if(client == NULL){
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

ClusterMigrate::ClusterMigrate(){
	src = NULL;
	dst = NULL;
}

ClusterMigrate::~ClusterMigrate(){
	delete src;
	delete dst;
}

int ClusterMigrate::check_version(ssdb::Client *client){
	const std::vector<std::string>* resp;
	resp = client->request("version");
	if(!resp || resp->size() < 2 || resp->at(0) != "ok"){
		log_error("ssdb-server 1.9.0 or higher is required!");
		return -1;
	}
	return 0;
}

int ClusterMigrate::move_key(const std::string &key){
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
	// TODO: TTL
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
	return (int)key.size() + (int)val.size();
}

int64_t ClusterMigrate::move_range(const std::string &max_key, std::string *moved_max_key, int num_keys){
	// get key range
	std::vector<std::string> keys;
	ssdb::Status s;
	// 从 "" 开始遍历, 是因为在中断之后, 重新执行时, 之前被中断了的数据是可以被迁移的. 
	s = src->keys("", max_key, num_keys, &keys);
	if(!s.ok()){
		log_error("response error: %s", s.code().c_str());
		return -1;
	}
	if(keys.empty()){
		return 0;
	}

	*moved_max_key = keys[keys.size() - 1];
	
	KeyRange new_src_range(*moved_max_key, max_key);
	log_info("new src: %s", new_src_range.str().c_str());
	s = src->set_kv_range(*moved_max_key, max_key);
	if(!s.ok()){
		log_error("src server set_kv_range error! %s", s.code().c_str());
		return -1;
	}

	int64_t bytes = 0;
	while(1){
		// move key range
		for(int i=0; i<(int)keys.size(); i++){
			const std::string &key = keys[i];
			int ret = move_key(key);
			if(ret == -1){
				log_error("move key %s error! %s", key.c_str(), s.code().c_str());
				return -1;  
			}
			bytes += ret;
		}
		
		keys.clear();

		s = src->keys("", *moved_max_key, num_keys, &keys);
		if(!s.ok()){
			log_error("response error: %s", s.code().c_str());
			return -1;
		}
		if(keys.empty()){
			break;
		}
	}
	return bytes;
}

int64_t ClusterMigrate::migrate_kv_data(Node *src_node, Node *dst_node, int num_keys){
	src = init_client(src_node->ip, src_node->port);
	if(src == NULL){
		log_error("failed to connect to server!");
		return -1;
	}
	dst = init_client(dst_node->ip, dst_node->port);
	if(dst == NULL){
		log_error("failed to connect to server!");
		return -1;
	}
	
	if(check_version(src) == -1){
		return -1;
	}
	if(check_version(dst) == -1){
		return -1;
	}
	
	ssdb::Status s;
	KeyRange src_range = src_node->range;
	KeyRange dst_range = dst_node->range;
	
	log_info("old src %s", src_range.str().c_str());
	log_info("old dst %s", dst_range.str().c_str());

	std::string moved_max_key;
	int64_t bytes;
	bytes = move_range(src_range.end, &moved_max_key, num_keys);
	if(bytes == -1){
		return -1;
	}
	if(bytes == 0){
		return 0;
	}

	// update key range
	src_node->range = KeyRange(moved_max_key, src_range.end);
	{
		dst_node->range = KeyRange(dst_range.begin, moved_max_key);
		log_info("new dst: %s", dst_node->range.str().c_str());
		ssdb::Status s = dst->set_kv_range(dst_node->range.begin, dst_node->range.end);
		if(!s.ok()){
			log_fatal("dst server set_kv_range error!");
			return -1;
		}
	}
	return bytes;
}
