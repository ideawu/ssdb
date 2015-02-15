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

int Cluster::init(){
	std::string work_dir = "./tmp";
	Options opt;
	db = SSDB::open(opt, work_dir);
	if(this->db == NULL){
		log_error("failed to open cluster db!");
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
	log_info("connected to %s", node->str().c_str());
	// TODO: 从 binlog 中, 获取 node.kv_range, 因为从 cluster 的角度看,
	// 节点在 cluster 中登记的 kv_range 可能和节点自身所认为的不一致, 以在
	// cluster 中登记的为准.
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
	log_debug("add kv node: %s", node->str().c_str());
	std::map<std::string, Node *>::iterator it;
	for(it = kv_node_list.begin(); it != kv_node_list.end(); it++){
		Node *n = it->second;
		if(node->kv_range.check_overlapped(n->kv_range)){
			log_error("overlapped with %s!", n->str().c_str());
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
	//log_debug("%s", __FUNCTION__);
	dst->kv_range = KeyRange();
	return _migrate_kv_data(src, dst);
}

int Cluster::migrate_kv_data(Node *src, Node *dst){
	//log_debug("%s", __FUNCTION__);
	return _migrate_kv_data(src, dst);
}

int Cluster::_migrate_kv_data(Node *src, Node *dst){
	Spliter spliter(this->db, src, dst);
	int64_t size = spliter.move_some();
	if(size == -1){
		log_error("error!");
		return -1;
	}
	if(size == 0){
		log_info("no data to move, end.");
		return 0;
	}
	log_debug("moved: %" PRId64 " bytes", size);
	
	del_kv_node(src);
	if(dst->id){
		del_kv_node(dst);
	}

	spliter.finish();

	add_kv_node(src);
	add_kv_node(dst);
	
	return 0;
}
