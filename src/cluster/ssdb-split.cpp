/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "split.h"

void welcome(){
	printf("ssdb-split - Split ssdb-server\n");
	printf("Copyright (c) 2012-2015 ssdb.io\n");
	printf("\n");
}

void usage(int argc, char **argv){
	printf("Usage:\n");
	printf("    %sr\n", argv[0]);
	printf("\n");
	printf("Options:\n");
	printf("    xx - \n");
}

int main(int argc, char **argv){
	welcome();
	
	Split split;
	split.init("127.0.0.1", 9000, "127.0.0.1", 8888, "127.0.0.1", 8889);
	
	std::string min_key;
	std::string max_key;
	while(1){
		int64_t size = split.move_some();
		if(size == -1){
			fprintf(stderr, "error!\n");
			exit(1);
		}
		if(size == 0){
			fprintf(stderr, "end.\n");
			split.finish();
			break;
		}
		printf("move %d bytes\n", (int)size);
	}
	
	return 0;
}

