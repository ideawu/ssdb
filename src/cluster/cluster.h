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
	std::map<std::string, Node *> kv_node_list;

	Cluster();
	~Cluster();
	
	int init(SSDB *db);
	Node* connect_node(const std::string &ip, int port);
	
	Node* get_node(int id);
	int add_kv_node(Node *node);
	int del_kv_node(Node *node);
	
	// 将 src 的数据分裂出一部分到 dst, 同时把 dst 加入到 node_list.
	int64_t split_kv_node(Node *src, Node *dst);
	// 将 src 的一部分数据移动到 dst, src 和 dst 的 key_range 必须是相连的.
	int64_t migrate_kv_data(Node *src, Node *dst);

	// TODO: for testing
	void print_node_list();
private:
	friend class ClusterStore;

	int last_node_id;
	SSDB *db;
	ClusterStore *store;

	int64_t _migrate_kv_data(Node *src, Node *dst);

	// 保存 node.kv_range.min_key 对应 node 的关系, 利用 map 的有序性.
	std::map<int, Node *> kv_nodes_by_id;
};

#endif
