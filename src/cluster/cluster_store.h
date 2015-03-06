#ifndef SSDB_CLUSTER_STORE_H
#define SSDB_CLUSTER_STORE_H

#include <string>

class Node;
class Cluster;

class ClusterStore{
public:
	ClusterStore(Cluster *cluster);
	~ClusterStore();
	
	int save_node(const Node *node);
	int del_node(const Node *node);
	int load_node_list();
	
private:
	Cluster *cluster;
	std::string node_list_key;

	int load_node(const std::string &id, const std::string &data);
};

#endif
