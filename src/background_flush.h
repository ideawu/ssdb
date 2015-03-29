/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef SSDB_BACKGROUND_FLUSH_H_
#define SSDB_BACKGROUND_FLUSH_H_

#include "include.h"
#include "util/config.h"

class SSDBServer;

class BackgroundFlush{
public:
	BackgroundFlush(SSDBServer *serv, const Config *config);
	~BackgroundFlush();
	void start();
    void end();
private:
	struct run_arg{
		const BackgroundFlush *background;
	};
	static void* _run_thread(void *arg);
	SSDBServer *_serv;
    const Config *_conf;
    pthread_t tid;
};

#endif
