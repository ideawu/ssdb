/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef SSDB_CLUSTER_STORE_H_
#define SSDB_CLUSTER_STORE_H_

#include "cluster.h"

class SSDB;

class ClusterStore
{
public:
	ClusterStore(SSDB *db);
	~ClusterStore();

	int save_kv_node(const Node &node);
	int load_kv_node(int id, Node *node);
	int del_kv_node(int id);
	int load_kv_node_list(std::vector<Node> *list);

private:
	SSDB *db;
	std::string kv_node_list_key;
};

#endif
