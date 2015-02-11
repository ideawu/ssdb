#include "cluster.h"
#include "key_range.h"
#include "node.h"
#include "spliter.h"
#include "../util/log.h"

Cluster::Cluster(){
	last_node_id = 1;
	this->db = NULL;
}

Cluster::~Cluster(){
	delete db;
}

int Cluster::init(const std::string &ip, int port){
	this->db = ssdb::Client::connect(ip, port);
	if(this->db == NULL){
		log_error("failed to connect to cluster server!");
		return -1;
	}
	return 0;
}

Node* Cluster::connect_node(const std::string &ip, int port){
	Node *node = new Node();
	if(node->init(ip, port) == -1){
		delete node;
		return NULL;
	}
	node->id = last_node_id ++;
	log_info("connected to %s", node->str().c_str());
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
	log_debug("add kv node: %s", node->str().c_str());
	std::map<std::string, Node *>::iterator it;
	for(it = kv_node_list.begin(); it != kv_node_list.end(); it++){
		Node *n = it->second;
		log_debug("%s", n->str().c_str());
		if(node->kv_range.check_overlapped(n->kv_range)){
			log_error("overlapped");
			return -1;
		}
	}
	kv_node_list[node->kv_range.start] = node;
	kv_nodes_by_id[node->id] = node;
	return 0;
}

int Cluster::del_kv_node(Node *node){
	kv_node_list.erase(node->kv_range.start);
	kv_nodes_by_id.erase(node->id);
	return 0;
}

int Cluster::split_kv_node(Node *src, Node *dst){
	Spliter spliter(this->db, src, dst);
	int64_t size = spliter.move_some();
	if(size == -1){
		fprintf(stderr, "error!\n");
		return -1;
	}
	if(size == 0){
		fprintf(stderr, "no data to move, end.\n");
		return -1;
	}
	log_debug("moved: %d", (int)size);
	spliter.finish();
	
	del_kv_node(src);
	add_kv_node(dst);
	add_kv_node(src);
	
	return 0;
}
