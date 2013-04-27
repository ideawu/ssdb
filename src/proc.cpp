#include "version.h"
#include "proc.h"
#include "t_kv.h"
#include "t_hash.h"
#include "t_zset.h"
#include "util/log.h"
#include "util/strings.h"

template<class T>
static std::string serialize_req(T &req){
	std::string ret;
	char buf[50];
	for(int i=0; i<req.size(); i++){
		if(i >= 5 && i < req.size() - 1){
			sprintf(buf, "[%d more...]", (int)req.size() - i);
			ret.append(buf);
			break;
		}
		if(((req[0] == "get" || req[0] == "set") && i == 1) || req[i].size() < 30){
			std::string h = hexmem(req[i].data(), req[i].size());
			ret.append(h);
		}else{
			sprintf(buf, "[%d bytes]", (int)req[i].size());
			ret.append(buf);
		}
		if(i < req.size() - 1){
			ret.append(" ");
		}
	}
	return ret;
}

CommandProc::CommandProc(SSDB *ssdb){
	this->ssdb = ssdb;
	backend_dump = new BackendDump(ssdb);
	backend_sync = new BackendSync(ssdb);

#define PROC(c) proc_map[#c] = &CommandProc::proc_##c
	PROC(get);
	PROC(set);
	PROC(del);
	PROC(incr);
	PROC(decr);
	PROC(scan);
	PROC(rscan);
	PROC(keys);
	PROC(exists);
	PROC(multi_exists);
	PROC(multi_get);
	PROC(multi_set);
	PROC(multi_del);

	PROC(hsize);
	PROC(hget);
	PROC(hset);
	PROC(hdel);
	PROC(hincr);
	PROC(hdecr);
	PROC(hscan);
	PROC(hrscan);
	PROC(hkeys);
	PROC(hlist);
	PROC(hexists);
	PROC(multi_hexists);
	PROC(multi_hsize);
	PROC(multi_hget);
	PROC(multi_hset);
	PROC(multi_hdel);

	PROC(zsize);
	PROC(zget);
	PROC(zset);
	PROC(zdel);
	PROC(zincr);
	PROC(zdecr);
	PROC(zscan);
	PROC(zrscan);
	PROC(zkeys);
	PROC(zlist);
	PROC(zexists);
	PROC(multi_zexists);
	PROC(multi_zsize);
	PROC(multi_zget);
	PROC(multi_zset);
	PROC(multi_zdel);

	PROC(dump);
	PROC(sync);
	PROC(info);
#undef PROC
}

CommandProc::~CommandProc(){
	delete backend_dump;
	delete backend_sync;
	log_debug("CommandProc finalized");
}

int CommandProc::proc(const Link &link, const Request &req, Response *resp){
	if(req.size() <= 0){
		return -1;
	}
	log_debug("req: %s", serialize_req(req).c_str());

	int ret = 0;
	proc_map_t::iterator it = proc_map.find(req[0]);
	if(it == proc_map.end()){
		resp->push_back("client_error");
		resp->push_back("Unknown Command: " + req[0].String());
	}else{
		proc_t p = it->second;
		ret = (this->*p)(link, req, resp);
	}

	if(ret != PROC_BACKEND){
		std::string log_buf = serialize_req(*resp);
		log_debug("req: %s, resp: %s", req[0].String().c_str(), serialize_req(*resp).c_str());
	}

	return ret;
}


int CommandProc::proc_dump(const Link &link, const Request &req, Response *resp){
	backend_dump->proc(&link);
	return PROC_BACKEND;
}

int CommandProc::proc_sync(const Link &link, const Request &req, Response *resp){
	backend_sync->proc(&link);
	return PROC_BACKEND;
}

int CommandProc::proc_info(const Link &link, const Request &req, Response *resp){
	resp->push_back("ok");
	resp->push_back("ssdb-server");
	resp->push_back("version");
	resp->push_back(SSDB_VERSION);

	std::vector<std::string> tmp = ssdb->info();
	for(int i=0; i<(int)tmp.size(); i++){
		std::string block = tmp[i];
		resp->push_back(block);
	}
	return 0;
}

#include "proc_kv.cpp"
#include "proc_hash.cpp"
#include "proc_zset.cpp"

