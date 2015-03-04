#ifndef SSDB_CLUSTER_H
#define SSDB_CLUSTER_H

#include <map>
#include <string>
#include "key_range.h"
#include "node.h"

class SSDB;
class ClusterStore;

class Cluster{
public:
	Cluster();
	~Cluster();
	
	int init();
	Node* connect_node(const std::string &ip, int port);
	
	int add_kv_node(Node *node);
	int del_kv_node(Node *node);
	
	// 将 src 的数据分裂出一部分到 dst, 同时把 dst 加入到 node_list.
	int split_kv_node(Node *src, Node *dst);
	// 将 src 的一部分数据移动到 dst, src 和 dst 的 key_range 必须是相连的.
	int migrate_kv_data(Node *src, Node *dst);

	// TODO: for testing
	void print_node_list();
	Node* get_node(int id){
		std::map<int, Node *>::iterator it;
		it = kv_nodes_by_id.find(id);
		if(it != kv_nodes_by_id.end()){
			return it->second;
		}
		return NULL;
	}
private:
	friend class ClusterStore;

	int last_node_id;
	SSDB *db;
	ClusterStore *store;

	int _migrate_kv_data(Node *src, Node *dst);

	// 保存 node.kv_range.min_key 对应 node 的关系, 利用 map 的有序性.
	std::map<std::string, Node *> kv_node_list;
	std::map<int, Node *> kv_nodes_by_id;
};

#endif
