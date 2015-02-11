#ifndef SSDB_CLUSTER_NODE_H
#define SSDB_CLUSTER_NODE_H

#include <string>
#include "key_range.h"
#include "SSDB_client.h"

class Node{
public:
	int id;
	KeyRange kv_range;
	ssdb::Client *db;

	Node();
	~Node();
	int init(const std::string &ip, int port);
	
	std::string str();
private:
};

#endif
