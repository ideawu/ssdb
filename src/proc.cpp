#include "version.h"
#include "util/log.h"
#include "util/strings.h"
#include "proc.h"
#include "t_kv.h"
#include "t_hash.h"
#include "t_zset.h"

#define PROC(c, f) {#c, f, 0, &CommandProc::proc_##c, 0, 0}
static CommandProc::Command commands[] = {
	PROC(info, "r"),
		
	PROC(get, "r"),
	PROC(set, "w"),
	PROC(del, "w"),
	PROC(incr, "w"),
	PROC(decr, "w"),
	PROC(scan, "r"),
	PROC(rscan, "r"),
	PROC(keys, "r"),
	PROC(exists, "r"),
	PROC(multi_exists, "r"),
	PROC(multi_get, "r"),
	PROC(multi_set, "w"),
	PROC(multi_del, "w"),

	PROC(hsize, "r"),
	PROC(hget, "r"),
	PROC(hset, "w"),
	PROC(hdel, "w"),
	PROC(hincr, "w"),
	PROC(hdecr, "w"),
	PROC(hscan, "r"),
	PROC(hrscan, "r"),
	PROC(hkeys, "r"),
	PROC(hlist, "r"),
	PROC(hexists, "r"),
	PROC(multi_hexists, "r"),
	PROC(multi_hsize, "r"),
	PROC(multi_hget, "r"),
	PROC(multi_hset, "w"),
	PROC(multi_hdel, "w"),

	PROC(zsize, "r"),
	PROC(zget, "r"),
	PROC(zset, "w"),
	PROC(zdel, "w"),
	PROC(zincr, "w"),
	PROC(zdecr, "w"),
	PROC(zscan, "r"),
	PROC(zrscan, "r"),
	PROC(zkeys, "r"),
	PROC(zlist, "r"),
	PROC(zexists, "r"),
	PROC(multi_zexists, "r"),
	PROC(multi_zsize, "r"),
	PROC(multi_zget, "r"),
	PROC(multi_zset, "w"),
	PROC(multi_zdel, "w"),

	PROC(dump, "b"),
	PROC(sync, "b"),

	{NULL, NULL, 0, NULL}
};
#undef PROC

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

	for(Command *cmd=commands; cmd->name; cmd++){
		for(const char *p=cmd->sflags; *p!='\0'; p++){
			switch(*p){
				case 'r':
					cmd->flags |= FLAG_READ;
					break;
				case 'w':
					cmd->flags |= FLAG_WRITE;
					break;
				case 'b':
					cmd->flags |= FLAG_BACKEND;
					break;
			}
		}
		proc_map[cmd->name] = cmd;
	}
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
	double stime = microtime();

	int ret = 0;
	Command *cmd = NULL;
	proc_map_t::iterator it = proc_map.find(req[0]);
	if(it == proc_map.end()){
		resp->push_back("client_error");
		resp->push_back("Unknown Command: " + req[0].String());
	}else{
		cmd = it->second;
		proc_t p = cmd->proc;
		ret = (this->*p)(link, req, resp);
	}

	double ts = 1000 *(microtime() - stime);
	log_debug("time: %.3f, req: %s, resp: %s", ts,
		 serialize_req(req).c_str(),
		 serialize_req(*resp).c_str());
	if(cmd){
		cmd->calls += 1;
		cmd->ts += ts;
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
	
	for(Command *cmd=commands; cmd->name; cmd++){
		char buf[64];
		snprintf(buf, sizeof(buf), "cmd.%s", cmd->name);
		resp->push_back(buf);
		snprintf(buf, sizeof(buf), "%llu %.0f", cmd->calls, cmd->ts);
		resp->push_back(buf);
	}

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

