/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef SSDB_BACKEND_DUMP_H_
#define SSDB_BACKEND_DUMP_H_

#include "include.h"
#include "ssdb/ssdb.h"
#include "net/link.h"

class BackendDump{
private:
	struct run_arg{
		const Link *link;
		const BackendDump *backend;
	};
	static void* _run_thread(void *arg);
	SSDB *ssdb;
public:
	BackendDump(SSDB *ssdb);
	~BackendDump();
	void proc(const Link *link);
};

#endif
