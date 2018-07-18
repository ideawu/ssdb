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
#include "thread.h"

// g++ log.o a.cpp

class MyWorker : public WorkerPool<MyWorker, int*>::Worker{
public:
	MyWorker(const std::string &name){
	}
	
	int proc(int *job){
		usleep(200 * 1000);
		*job = 100000 + *job;
		return 0;
	}
};

#define NUM_JOBS 10

int main(){
	int jobs[NUM_JOBS];
	WorkerPool<MyWorker, int*> tp("test");
	tp.start(3);
	
	log_debug("add begin");
	for(int i=0; i<NUM_JOBS; i++){
		//usleep(200 * 1000);
		log_debug("    add job: %d", i);
		jobs[i] = i;
		tp.push(&jobs[i]);
	}
	log_debug("add end");
	
	log_debug("pop begin");
	for(int i=0; i<NUM_JOBS-1; i++){
		int *job;
		tp.pop(&job);
		log_debug("    result: %d, %d", i, *job);
	}
	log_debug("pop end");
	
	log_debug("stopping...");
	tp.stop();
	log_debug("stopped");

	return 0;
}
