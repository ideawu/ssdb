/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef NET_WORKER_H_
#define NET_WORKER_H_

#include <string>
#include "../util/thread.h"
#include "proc.h"

// WARN: pipe latency is about 20 us, it is really slow!
class ProcWorker : public WorkerPool<ProcWorker, ProcJob>::Worker{
public:
	ProcWorker(const std::string &name);
	~ProcWorker(){}
	void init();
	int proc(ProcJob *job);
};

typedef WorkerPool<ProcWorker, ProcJob> ProcWorkerPool;

#endif
