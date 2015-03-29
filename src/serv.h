/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef SSDB_SERVER_H_
#define SSDB_SERVER_H_

#include "include.h"
#include <map>
#include <vector>
#include <string>
#include "ssdb/ssdb_impl.h"
#include "ssdb/ttl.h"
#include "backend_dump.h"
#include "backend_sync.h"
#include "background_flush.h"
#include "slave.h"
#include "net/server.h"


class SSDBServer
{
private:
	void reg_procs(NetworkServer *net);
	
	std::string kv_range_s;
	std::string kv_range_e;
    
    std::string meta_dir;
    std::string data_dir;

	SSDB *meta;

public:
	SSDBImpl *ssdb;
	BackendDump *backend_dump;
	BackendSync *backend_sync;
    BackgroundFlush *background_flush;
	ExpirationHandler *expiration;
	std::vector<Slave *> slaves;

	SSDBServer(SSDB *ssdb, SSDB *meta, const std::string &data_db_dir, const std::string &data_meta_dir, const Config &conf, NetworkServer *net);
	~SSDBServer();
    
    const char *get_meta_dir();
    const char *get_data_dir();

	int set_kv_range(const std::string &s, const std::string &e);
	int get_kv_range(std::string *s, std::string *e);
	bool in_kv_range(const std::string &key);
	bool in_kv_range(const Bytes &key);
};


#define CHECK_KEY_RANGE(n) do{ \
		if(req.size() > n){ \
			if(!serv->in_kv_range(req[n])){ \
				resp->push_back("out_of_range"); \
				return 0; \
			} \
		} \
	}while(0)

#define CHECK_NUM_PARAMS(n) do{ \
		if(req.size() < n){ \
			resp->push_back("client_error"); \
			resp->push_back("wrong number of arguments"); \
			return 0; \
		} \
	}while(0)

#endif
