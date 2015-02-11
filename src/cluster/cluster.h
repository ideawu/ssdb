#ifndef SSDB_CLUSTER_H
#define SSDB_CLUSTER_H

#include <map>
#include <string>
#include "key_range.h"
#include "node.h"

class Cluster{
public:
	Cluster();
	~Cluster();

	// 保存 node.kv_range.min_key 对应 node 的关系, 利用 map 的有序性.
	std::map<std::string, Node *> kv_node_list;
	
	int init_kv_node(Node *node);
	int add_kv_node(Node *node);
	int del_kv_node(Node *node);

	void print_node_list();
private:
};

#endif
