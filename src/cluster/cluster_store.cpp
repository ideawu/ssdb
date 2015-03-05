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
	int ret = 0;
	HIterator *it = cluster->db->hrscan(node_list_key, "", "", 10000);
	if(it){
		while(it->next()){
			ret = this->load_node(it->key, it->val);
			if(ret == -1){
				break;
			}
		}
		delete it;
	}
	return ret;
}

int ClusterStore::load_node(const std::string &id, const std::string &data){
	std::string ip, kv_start, kv_end;
	int port;
	
	LineDecoder dec(data);
	dec.read(&ip);
	dec.read(&port);
	dec.read(&kv_start);
	dec.read(&kv_end);
	
	log_debug("load node[%s - %s:%d] \"%s\" \"%s\"", id.c_str(), ip.c_str(), port,
		 str_escape(kv_start).c_str(), str_escape(kv_end).c_str());

	Node *node = cluster->connect_node(ip, port);
	if(!node){
		log_error("load node[%s:%d] error!", ip.c_str(), port);
		return -1;
	}
	
	// 从 binlog 中, 获取 id 和 node.kv_range, 因为从 cluster 的角度看,
	// 节点在 cluster 中登记的 kv_range 可能和节点自身所认为的不一致, 以在
	// cluster 中登记的为准.
	node->id = str_to_int(id);
	if(node->id > cluster->last_node_id){
		cluster->last_node_id = node->id;
	}
	
	if(node->set_kv_range(KeyRange(kv_start, kv_end)) == -1){
		log_error("init_kv_range error!");
	}
	
	if(cluster->add_kv_node(node) == -1){
		log_error("init_kv_node error!");
		return -1;
	}

	return 0;
}
