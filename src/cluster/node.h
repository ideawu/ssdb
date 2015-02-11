#ifndef SSDB_CLUSTER_NODE_H
#define SSDB_CLUSTER_NODE_H

#include "key_range.h"

class Node{
public:
	Node(){
	}
	KeyRange kv_range;
};

#endif
