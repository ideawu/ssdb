#ifndef SSDB_TTL_H_
#define SSDB_TTL_H_

#include "include.h"
#include "ssdb.h"
#include "util/sorted_set.h"
#include "util/thread.h"
#include <string>

class ExpirationHandler
{
public:
	ExpirationHandler(SSDB *ssdb);
	~ExpirationHandler();
	int set_ttl(const Bytes &key, int64_t ttl);

private:
	SSDB *ssdb;
	volatile bool thread_quit;
	std::string list_name;
	SortedSet expiration_keys;
	Mutex mutex;

	void start();
	void stop();
	static void* thread_func(void *arg);
};

#endif
