#ifndef SSDB_BACKEND_SYNC_H_
#define SSDB_BACKEND_SYNC_H_

#include "include.h"
#include <vector>
#include <string>
#include <map>

#include "ssdb.h"
#include "link.h"
#include "binlog.h"
#include "util/lock.h"
#include "util/thread.h"

class BackendSync{
	private:
		struct Client;
	private:
		std::vector<Client *> clients;
		std::vector<Client *> clients_tmp;

		struct run_arg{
			const Link *link;
			const BackendSync *backend;
		};
		volatile bool thread_quit;
		Mutex mutex;
		std::map<pthread_t, pthread_t> workers;
		static void* _run_thread(void *arg);
		const SSDB *ssdb;
	public:
		BackendSync(const SSDB *ssdb);
		~BackendSync();
		void proc(const Link *link);
};

struct BackendSync::Client{
	static const int INIT = 0;
	static const int OUT_OF_SYNC = 1;
	static const int DUMP = 2;
	static const int SYNC = 4;

	int status;
	Link *link;
	uint64_t last_seq;
	uint64_t last_noop_seq;
	std::string last_key;
	const BackendSync *backend;
	bool is_mirror;

	Client(const BackendSync *backend);
	~Client();
	void init();
	void reset();
	void noop();
	int dump();
	int sync(BinlogQueue *logs);
};

#endif
