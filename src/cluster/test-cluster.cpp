/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "node.h"
#include "cluster.h"
#include "../util/log.h"
#include "../ssdb/ssdb.h"

int main(int argc, char **argv){
	std::string work_dir = "./tmp";
	Options opt;
	SSDB *db = SSDB::open(opt, work_dir);
	if(db == NULL){
		log_error("failed to open cluster db!");
		return -1;
	}

	Cluster cluster;
	cluster.init(db);
	
	Node *node1, *node2;
	
	if(0){
		node1 = cluster.connect_node("127.0.0.1", 8888);
		node2 = cluster.connect_node("127.0.0.1", 8889);
		if(!node1 || !node2){
			printf("init node error!\n");
			return 0;
		}
	
		cluster.add_kv_node(node1);
		printf("node_list:\n");
		cluster.print_node_list();

		printf("press Enter to init nodes: ");
		getchar();

		cluster.split_kv_node(node1, node2);
		printf("node_list:\n");
		cluster.print_node_list();
		return 0;
	}else{
		printf("init node_list:\n");
		cluster.print_node_list();

		node1 = cluster.get_node(1);
		node2 = cluster.get_node(2);
		if(!node1 || !node2){
			printf("init node error!\n");
			return 0;
		}
	
		while(1){
			printf("Press Enter to migrate_kv_data: ");
			getchar();
			cluster.migrate_kv_data(node1, node2);
			printf("node_list:\n");
			cluster.print_node_list();
		}
	}
	
	return 0;
}

