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
	Node node1, node2;
	node1.kv_range.end = "c";
	node2.kv_range.start = "c";
	cluster.init_kv_node(&node1);
	cluster.init_kv_node(&node2);
	
	printf("node_list:\n");
	cluster.print_node_list();
	return 0;
}

