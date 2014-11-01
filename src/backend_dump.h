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
