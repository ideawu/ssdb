#include "cluster.h"
#include "key_range.h"
#include "node.h"
#include "../util/log.h"

Cluster::Cluster(){
}

Cluster::~Cluster(){
}

int Cluster::init_kv_node(Node *node){
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
	return 0;
}

void Cluster::print_node_list(){
	std::map<std::string, Node *>::iterator it;
	int num = 0;
	for(it = kv_node_list.begin(); it != kv_node_list.end(); it++){
		Node *n = it->second;
		printf("%4d: (\"%s\" - \"%s\"]\n", num, n->kv_range.start.c_str(), n->kv_range.end.c_str());
		num ++;
	}
}
