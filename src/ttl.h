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
	// "In Redis 2.6 or older the command returns -1 if the key does not exist
	// or if the key exist but has no associated expire. Starting with Redis 2.8.."
	// I stick to Redis 2.6
	int64_t get_ttl(const Bytes &key);

private:
	SSDB *ssdb;
	volatile bool thread_quit;
	std::string list_name;
	SortedSet expiration_keys;
	Mutex mutex;

	void start();
	void stop();
	static void* thread_func(void *arg);
	void load_expiration_keys_from_db(int num);
};

#endif
