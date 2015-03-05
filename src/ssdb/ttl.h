/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef SSDB_TTL_H_
#define SSDB_TTL_H_

#include "ssdb.h"
#include "../util/thread.h"
#include "../util/sorted_set.h"
#include <string>

class ExpirationHandler
{
public:
	Mutex mutex;

	ExpirationHandler(SSDB *ssdb);
	~ExpirationHandler();

	// "In Redis 2.6 or older the command returns -1 if the key does not exist
	// or if the key exist but has no associated expire. Starting with Redis 2.8.."
	// I stick to Redis 2.6
	int64_t get_ttl(const Bytes &key);
	// The caller must hold mutex before calling set/del functions
	int del_ttl(const Bytes &key);
	int set_ttl(const Bytes &key, int64_t ttl);

private:
	SSDB *ssdb;
	volatile bool thread_quit;
	std::string list_name;
	int64_t first_timeout;
	SortedSet fast_keys;

	void start();
	void stop();
	void expire_loop();
	static void* thread_func(void *arg);
	void load_expiration_keys_from_db(int num);
};

#endif
