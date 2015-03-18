#include "cluster_store.h"
#include "ssdb/ssdb.h"
#include "util/log.h"
#include "util/line.h"

ClusterStore::ClusterStore(SSDB *db){
	this->db = db;
	this->kv_node_list_key = "KV_NODE_LIST";
}

ClusterStore::~ClusterStore(){
}

int ClusterStore::save_kv_node(const Node &node){
	LineEncoder enc;
	enc.write(node.id);
	enc.write(node.status);
	enc.write(node.range.begin);
	enc.write(node.range.end);
	enc.write(node.ip);
	enc.write(node.port);

	std::string key = str(node.id);
	std::string val = enc.str();
	int ret = db->hset(kv_node_list_key, key, val);
	if(ret == -1){
		log_error("cluster store error!");
		return -1;
	}
	return 0;
}

int ClusterStore::load_kv_node(int id, Node *node){
	std::string key = str(id);
	std::string val;
	int ret = db->hget(kv_node_list_key, key, &val);
	if(ret <= 0){
		return ret;
	}
	LineDecoder dec(val);
	dec.read(&node->id);
	dec.read(&node->status);
	dec.read(&node->range.begin);
	dec.read(&node->range.end);
	dec.read(&node->ip);
	dec.read(&node->port);
	return 1;
}

int ClusterStore::del_kv_node(int id){
	std::string key = str(id);
	int ret = db->hdel(kv_node_list_key, key);
	if(ret == -1){
		log_error("cluster store error!");
		return -1;
	}
	return ret;
}

int ClusterStore::load_kv_node_list(std::vector<Node> *list){
	int ret = 0;
	HIterator *it = db->hscan(kv_node_list_key, "", "", 10000);
	if(it){
		while(it->next()){
			Node node;
			this->load_kv_node(str_to_int(it->key), &node);
			log_debug("load node: %d - %s %s:%d", node.id, node.range.str().c_str(),
				node.ip.c_str(), node.port);
			list->push_back(node);
		}
		delete it;
	}
	return ret;
}
