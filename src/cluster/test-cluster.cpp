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

int main(int argc, char **argv){
	Cluster cluster;
	cluster.init();
	
	printf("init node_list:\n");
	cluster.print_node_list();
	getchar();

	Node *node1 = cluster.connect_node("127.0.0.1", 8888);
	Node *node2 = cluster.connect_node("127.0.0.1", 8889);
	
	cluster.add_kv_node(node1);
	printf("node_list:\n");
	cluster.print_node_list();

	cluster.split_kv_node(node1, node2);
	printf("node_list:\n");
	cluster.print_node_list();

	while(1){
		printf("Press Enter to migrate_kv_data: ");
		getchar();
		cluster.migrate_kv_data(node1, node2);
		printf("node_list:\n");
		cluster.print_node_list();
	}

	delete node1;
	delete node2;
	
	return 0;
}

