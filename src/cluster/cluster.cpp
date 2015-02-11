#include "cluster.h"
#include "key_range.h"
#include "node.h"
#include "../util/log.h"
#include "../util/strings.h"

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
	log_info("node#%d kv_range: (\"%s\", \"%s\"]", node->id,
			str_escape(node->kv_range.start).c_str(), str_escape(node->kv_range.end).c_str());
	return node;
}

void Cluster::print_node_list(){
	std::map<std::string, Node *>::iterator it;
	for(it = kv_node_list.begin(); it != kv_node_list.end(); it++){
		Node *n = it->second;
		printf("%4d: (\"%s\" - \"%s\"]\n", n->id, n->kv_range.start.c_str(), n->kv_range.end.c_str());
	}
}

int Cluster::add_kv_node(Node *node){
	std::map<std::string, Node *>::iterator it;
	for(it = kv_node_list.begin(); it != kv_node_list.end(); it++){
		Node *n = it->second;
		log_debug("(\"%s\" - \"%s\"]", n->kv_range.start.c_str(), n->kv_range.end.c_str());
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
	// TEST
	dst->kv_range = src->kv_range;
	if(src->kv_range.start.empty()){
		if(src->kv_range.end.empty()){
			src->kv_range.start = "p";
			dst->kv_range.end = "p";
		}else{
			char c = src->kv_range.end[0] - 5;
			std::string s(1, c);
			src->kv_range.start = s;
			dst->kv_range.end = s;
		}
	}else{
		char c = src->kv_range.start[0] + 5;
		std::string s(1, c);
		src->kv_range.start = s;
		dst->kv_range.end = s;
	}
	
	del_kv_node(src);
	add_kv_node(dst);
	add_kv_node(src);
	
	return 0;
}
