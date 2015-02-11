/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "spliter.h"
#include "node.h"
#include "SSDB_client.h"
#include "../util/log.h"

int main(int argc, char **argv){
	Node node1, node2;
	
	node1.init("127.0.0.1", 8888);
	node2.init("127.0.0.1", 8889);
	
	ssdb::Client *db = ssdb::Client::connect("127.0.0.1", 8887);
	if(db == NULL){
		log_error("failed to connect to cluster server!");
		return -1;
	}

	Spliter spliter(db, &node1, &node2);
	
	ssdb::Status s;
	int64_t src_dbsize;
	s = spliter.src_node->db->dbsize(&src_dbsize);
	if(!s.ok()){
		fprintf(stderr, "%d error!\n", __LINE__);
		exit(1);
	}
	
	printf("src dbsize: %lld\n", src_dbsize);
	
	int64_t total_moved = 0;
	std::string min_key;
	std::string max_key;
	while(1){
		int64_t size = spliter.move_some();
		if(size == -1){
			fprintf(stderr, "error!\n");
			exit(1);
		}
		if(size == 0){
			fprintf(stderr, "no data to move, end.\n");
			spliter.finish();
			break;
		}
		total_moved += size;
		
		int64_t src_dbsize_new;
		s = spliter.src_node->db->dbsize(&src_dbsize_new);
		if(!s.ok()){
			fprintf(stderr, "error!\n");
			exit(1);
		}
		
		int64_t dst_dbsize_new;
		s = spliter.dst_node->db->dbsize(&dst_dbsize_new);
		if(!s.ok()){
			fprintf(stderr, "error!\n");
			exit(1);
		}

		printf("moved: %lld, src_dbsize_old: %lld, src_dbsize_new: %lld, dst_dbsize_new: %lld\n",
			total_moved/1024, src_dbsize/1024, src_dbsize_new/1024, dst_dbsize_new/1024);
		
		if(total_moved > src_dbsize/2 || dst_dbsize_new > src_dbsize/2 || (src_dbsize_new - src_dbsize) > src_dbsize/10){
			fprintf(stderr, "split end.\n");
			spliter.finish();
			break;
		}
		
		getchar();
	}
	
	return 0;
}

