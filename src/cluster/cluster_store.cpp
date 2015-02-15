#include "cluster_store.h"
#include "node.h"
#include "cluster.h"
#include "../ssdb/ssdb.h"
#include "../util/log.h"
#include "../util/strings.h"
#include "../util/line.h"

ClusterStore::ClusterStore(Cluster *cluster){
	this->cluster = cluster;
	this->node_list_key = "node_list";
}

ClusterStore::~ClusterStore(){
}
	
int ClusterStore::save_node(const Node *node){
	// TODO: 需要设计一种更好的序列化方案, 或者使用 json?
	std::string node_key = str(node->id);
	std::string val;
	
	LineEncoder enc;
	enc.write(node->ip);
	enc.write(node->port);
	enc.write(node->kv_range.start);
	enc.write(node->kv_range.end);
	
	val = enc.str();
	
	int ret = cluster->db->hset(node_list_key, node_key, val);
	if(ret == -1){
		log_error("cluster store error!");
		return -1;
	}

	return 0;
}

int ClusterStore::load_node_list(){
	std::string node_key = str(1);
	std::string val;

	int ret = cluster->db->hget(node_list_key, node_key, &val);
	if(ret == -1){
		log_error("cluster store error!");
		return -1;
	}

	std::string ip, kv_start, kv_end;
	int port;
	
	LineDecoder dec(val);
	dec.read(&ip);
	dec.read(&port);
	dec.read(&kv_start);
	dec.read(&kv_end);
	
	log_debug("%s:%d %s %s", ip.c_str(), port, kv_start.c_str(), kv_end.c_str());

	return 0;
}
