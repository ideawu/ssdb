#ifndef SSDB_BACKEND_DUMP_H_
#define SSDB_BACKEND_DUMP_H_

#include "include.h"
#include "ssdb.h"
#include "link.h"

class BackendDump{
	private:
		struct run_arg{
			const Link *link;
			const BackendDump *backend;
		};
		static void* _run_thread(void *arg);
		const SSDB *ssdb;
	public:
		BackendDump(const SSDB *ssdb);
		~BackendDump();
		void proc(const Link *link);
};

#endif
