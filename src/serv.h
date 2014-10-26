#ifndef SSDB_SERVER_H_
#define SSDB_SERVER_H_

#include "include.h"
#include <map>
#include <vector>
#include <string>
#include "ssdb.h"
#include "backend_dump.h"
#include "backend_sync.h"
#include "ttl.h"
#include "slave.h"
#include "net/server.h"


class SSDBServer{
public:
	SSDB *ssdb;
	BackendDump *backend_dump;
	BackendSync *backend_sync;
	ExpirationHandler *expiration;
	std::vector<Slave *> slaves;

	SSDBServer(SSDB *ssdb, const Config &conf);
	~SSDBServer();
	
	void reg_procs(NetworkServer *net);
};


#define CHECK_KEY_RANGE(n) do{ \
		if(req.size() > 1){ \
			if(!serv->ssdb->in_kv_range(req[n])){ \
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
