#include "cluster.h"
#include "cluster_store.h"
#include "key_range.h"
#include "node.h"
#include "spliter.h"
#include "node.h"
#include "../ssdb/ssdb.h"
#include "../util/log.h"

static const int MOVE_BATCH_SIZE = 2;

Cluster::Cluster(){
	last_node_id = 1;
	this->db = NULL;
	this->store = NULL;
}

Cluster::~Cluster(){
	delete db;
	delete store;
}

int Cluster::init(){
	std::string work_dir = "./tmp";
	Options opt;
	this->db = SSDB::open(opt, work_dir);
	if(this->db == NULL){
		log_error("failed to open cluster db!");
		return -1;
	}
	
	this->store = new ClusterStore(this);
	this->store->load_node_list();
	return 0;
}

Node* Cluster::connect_node(const std::string &ip, int port){
	Node *node = new Node();
	if(node->init(ip, port) == -1){
		delete node;
		return NULL;
	}
	log_info("connected to [%s:%d]", node->ip.c_str(), node->port);
	return node;
}

void Cluster::print_node_list(){
	std::map<std::string, Node *>::iterator it;
	for(it = kv_node_list.begin(); it != kv_node_list.end(); it++){
		Node *n = it->second;
		printf("%s\n", n->str().c_str());
	}
}

int Cluster::add_kv_node(Node *node){
	if(!node->id){
		// TODO: 在别的地方分配 id
		node->id = last_node_id ++;
	}
	log_debug("%s: %s", __FUNCTION__, node->str().c_str());
	std::map<std::string, Node *>::iterator it;
	for(it = kv_node_list.begin(); it != kv_node_list.end(); it++){
		Node *n = it->second;
		if(node->kv_range.check_overlapped(n->kv_range)){
			log_error("overlapped with %s!", n->str().c_str());
			return -1;
		}
	}
	
	// log
	if(store->save_node(node) == -1){
		return -1;
	}
	
	kv_node_list[node->kv_range.start] = node;
	kv_nodes_by_id[node->id] = node;
	return 0;
}

int Cluster::del_kv_node(Node *node){
	log_debug("%s: %s", __FUNCTION__, node->str().c_str());
	kv_node_list.erase(node->kv_range.start);
	kv_nodes_by_id.erase(node->id);
	return 0;
}

int64_t Cluster::split_kv_node(Node *src, Node *dst){
	log_debug("%s: %s => %s", __FUNCTION__, src->str().c_str(), dst->str().c_str());
	dst->kv_range = KeyRange();
	return _migrate_kv_data(src, dst);
}

int64_t Cluster::migrate_kv_data(Node *src, Node *dst){
	log_debug("%s: %s => %s", __FUNCTION__, src->str().c_str(), dst->str().c_str());
	return _migrate_kv_data(src, dst);
}

int64_t Cluster::_migrate_kv_data(Node *src, Node *dst){
	Spliter spliter(this->db, src, dst);
	int64_t size = spliter.move_some(MOVE_BATCH_SIZE);
	if(size == -1){
		log_error("error!");
		return -1;
	}
	if(size == 0){
		log_info("no data to move, end.");
		return 0;
	}
	log_debug("moved: %" PRId64 " bytes", size);
	
	int ret;
	ret = del_kv_node(src);
	if(ret == -1){
		log_error("error!");
		return -1;
	}
	if(dst->id){
		ret = del_kv_node(dst);
		if(ret == -1){
			log_error("error!");
			return -1;
		}
	}

	ret = spliter.finish();
	if(ret == -1){
		log_error("error!");
		return -1;
	}

	ret = add_kv_node(src);
	if(ret == -1){
		log_error("error!");
		return -1;
	}
	
	ret = add_kv_node(dst);
	if(ret == -1){
		log_error("error!");
		return -1;
	}
	
	return size;
}
