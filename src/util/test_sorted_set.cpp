/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "log.h"
#include "sorted_set.h"

int main(int argc, char **argv){	
	SortedSet zset;

	std::vector<std::string> keys;
	for(int i='a'; i<='z'; i++){
		char buf[10];
		snprintf(buf, sizeof(buf), "%c", i);
		keys.push_back(buf);
	}
	
	log_debug("");
	srand(time(NULL));
	for(int i=0; i<1000 * 1000; i++){
		std::string &key = keys[rand() % keys.size()];
		zset.add(key, rand()%30 - 15);
	}
	log_debug("");
	
	std::string key;
	int64_t score;
	int n = 0;
	while(zset.front(&key, &score)){
		printf("%s : %4lld\n", key.c_str(), score);
		zset.pop_front();
		n ++;
	}
	log_debug("%d", n);
	
	return 0;
}
