/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "cluster.h"

int main(int argc, char **argv){
	Cluster cluster;
	cluster.init("127.0.0.1", 8887);
	
	Node *node1 = cluster.connect_node("127.0.0.1", 8888);
	Node *node2 = cluster.connect_node("127.0.0.1", 8889);
	
	cluster.add_kv_node(node1);
	printf("node_list:\n");
	cluster.print_node_list();

	cluster.split_kv_node(node1, node2);
	printf("node_list:\n");
	cluster.print_node_list();

	delete node1;
	delete node2;
	
	return 0;
}

