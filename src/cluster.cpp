/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "cluster.h"
#include "ssdb/ssdb.h"
#include "util/log.h"

Cluster::Cluster(){
	next_id = 1;
}

Cluster::~Cluster(){
}

Cluster* Cluster::create(SSDB *db){
	Cluster *cluster = new Cluster;
	cluster->db = db;
	
	// load node list
	return cluster;
}

int Cluster::add_node(const std::string &ip, int port){
	Node node;
	node.id = next_id ++;
	node.ip = ip;
	node.port = port;
	node_list.push_back(node);
	return node.id;
}

int Cluster::del_node(int id){
	std::vector<Node>::iterator it;
	for(it=node_list.begin(); it!=node_list.end(); it++){
		const Node &node = *it;
		if(node.id == id){
			node_list.erase(it);
			return 1;
		}
	}
	return 0;
}

int Cluster::set_key_range(int id, const KeyRange &range){
	std::vector<Node>::iterator it;
	for(it=node_list.begin(); it!=node_list.end(); it++){
		Node &node = *it;
		if(node.id == id){
			node.range = range;
			return 1;
		}
	}
	return 0;
}

int Cluster::get_node_list(std::vector<Node> *list){
	*list = node_list;
	return 0;
}

int64_t Cluster::migrate_data(int src_id, int dst_id, int keys){
	return -1;
}

