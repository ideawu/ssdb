#include "version.h"

#include "util/log.h"
#include "util/strings.h"
#include "serv.h"
#include "t_kv.h"
#include "t_hash.h"
#include "t_zset.h"

#define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
#if GCC_VERSION >= 403
	#include <tr1/unordered_map>
#else
	#include <ext/hash_map>
#endif


struct BytesEqual{
	bool operator()(const Bytes &s1, const Bytes &s2) const {
		return (bool)(s1.compare(s2) == 0);
	}
};
struct BytesHash{
	size_t operator()(const Bytes &s1) const {
		unsigned long __h = 0;
		const char *p = s1.data();
		for (int i=0 ; i<s1.size(); i++)
			__h = 5*__h + p[i];
		return size_t(__h);
	}
};

#if GCC_VERSION >= 403
		typedef std::tr1::unordered_map<Bytes, Command *, BytesHash, BytesEqual> proc_map_t;
#else
		typedef __gnu_cxx::hash_map<Bytes, Command *, BytesHash, BytesEqual> proc_map_t;
#endif

static proc_map_t proc_map;


#define DEF_PROC(f) static int proc_##f(Server *serv, Link *link, const Request &req, Response *resp)
	DEF_PROC(get);
	DEF_PROC(set);
	DEF_PROC(del);
	DEF_PROC(incr);
	DEF_PROC(decr);
	DEF_PROC(scan);
	DEF_PROC(rscan);
	DEF_PROC(keys);
	DEF_PROC(exists);
	DEF_PROC(multi_exists);
	DEF_PROC(multi_get);
	DEF_PROC(multi_set);
	DEF_PROC(multi_del);

	DEF_PROC(hsize);
	DEF_PROC(hget);
	DEF_PROC(hset);
	DEF_PROC(hdel);
	DEF_PROC(hincr);
	DEF_PROC(hdecr);
	DEF_PROC(hscan);
	DEF_PROC(hrscan);
	DEF_PROC(hkeys);
	DEF_PROC(hlist);
	DEF_PROC(hexists);
	DEF_PROC(multi_hexists);
	DEF_PROC(multi_hsize);
	DEF_PROC(multi_hget);
	DEF_PROC(multi_hset);
	DEF_PROC(multi_hdel);

	DEF_PROC(zsize);
	DEF_PROC(zget);
	DEF_PROC(zset);
	DEF_PROC(zdel);
	DEF_PROC(zincr);
	DEF_PROC(zdecr);
	DEF_PROC(zscan);
	DEF_PROC(zrscan);
	DEF_PROC(zkeys);
	DEF_PROC(zlist);
	DEF_PROC(zexists);
	DEF_PROC(multi_zexists);
	DEF_PROC(multi_zsize);
	DEF_PROC(multi_zget);
	DEF_PROC(multi_zset);
	DEF_PROC(multi_zdel);

	DEF_PROC(info);
	DEF_PROC(dump);
	DEF_PROC(sync140);
#undef DEF_PROC


#define PROC(c, f) {#c, f, 0, proc_##c, 0, 0, 0}
static Command commands[] = {
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

	PROC(info, "r"),
	PROC(dump, "b"),
	PROC(sync140, "b"),

	{NULL, NULL, 0, NULL}
};
#undef PROC

Server::Server(SSDB *ssdb){
	this->ssdb = ssdb;
	backend_dump = new BackendDump(ssdb);
	backend_sync = new BackendSync(ssdb);

	for(Command *cmd=commands; cmd->name; cmd++){
		for(const char *p=cmd->sflags; *p!='\0'; p++){
			switch(*p){
				case 'r':
					cmd->flags |= Command::FLAG_READ;
					break;
				case 'w':
					cmd->flags |= Command::FLAG_WRITE;
					break;
				case 'b':
					cmd->flags |= Command::FLAG_BACKEND;
					break;
			}
		}
		proc_map[cmd->name] = cmd;
	}
	// for k-v data, list === keys
	proc_map["list"] = proc_map["keys"];
	
	writer.start(MAX_WRITERS);
}

Server::~Server(){
	delete backend_dump;
	delete backend_sync;
	writer.stop();
	log_debug("CommandProc finalized");
}

void Server::proc(ProcJob *job){
	job->serv = this;
	job->result = PROC_OK;
	job->stime = millitime();
	const Request *req = job->link->last_recv();
	
	Response resp;
	proc_map_t::iterator it = proc_map.find(req->at(0));
	if(it == proc_map.end()){
		resp.push_back("client_error");
		resp.push_back("Unknown Command: " + req->at(0).String());
	}else{
		Command *cmd = it->second;
		job->cmd = cmd;
		if(cmd->flags & Command::FLAG_WRITE){
			job->result = PROC_BACKEND;
			writer.push(*job);
			return; /////
		}
		proc_t p = cmd->proc;
		job->time_wait = 1000 *(millitime() - job->stime);
		job->result = (*p)(this, job->link, *req, &resp);
		job->time_proc = 1000 *(millitime() - job->stime);
	}
	
	if(job->link->send(resp) == -1){
		job->result = PROC_ERROR;
	}else{
		log_debug("w:%.3f,p:%.3f, req: %s, resp: %s",
			job->time_wait, job->time_proc,
			serialize_req(*req).c_str(),
			serialize_req(resp).c_str());
	}
}



void Server::WriteProc::init(){
	log_debug("%d init", this->id);
}

void Server::WriteProc::destroy(){
	log_debug("%d destroy", this->id);
}

int Server::WriteProc::proc(ProcJob *job){
	const Request *req = job->link->last_recv();
	Response resp;
	
	double stime = millitime();
	proc_t p = job->cmd->proc;
	job->result = (*p)(job->serv, job->link, *req, &resp);
	double etime = millitime();
	job->time_wait = 1000 * (stime - job->stime);
	job->time_proc = 1000 *(etime - stime);

	if(job->link->send(resp) == -1){
		job->result = PROC_ERROR;
	}else{
		log_debug("wait: %.3f, proc: %.3f, req: %s, resp: %s",
			job->time_wait, job->time_proc,
			serialize_req(*req).c_str(),
			serialize_req(resp).c_str());
	}
	return 0;
}


static int proc_dump(Server *serv, Link *link, const Request &req, Response *resp){
	serv->backend_dump->proc(link);
	return PROC_BACKEND;
}

static int proc_sync140(Server *serv, Link *link, const Request &req, Response *resp){
	serv->backend_sync->proc(link);
	return PROC_BACKEND;
}

static int proc_info(Server *serv, Link *link, const Request &req, Response *resp){
	resp->push_back("ok");
	resp->push_back("ssdb-server");
	resp->push_back("version");
	resp->push_back(SSDB_VERSION);
	
	for(Command *cmd=commands; cmd->name; cmd++){
		char buf[128];
		snprintf(buf, sizeof(buf), "cmd.%s", cmd->name);
		resp->push_back(buf);
		snprintf(buf, sizeof(buf), "calls: %"PRIu64"\ttime_wait: %.0f\ttime_proc: %.0f",
			cmd->calls, cmd->time_wait, cmd->time_proc);
		resp->push_back(buf);
	}

	std::vector<std::string> tmp = serv->ssdb->info();
	for(int i=0; i<(int)tmp.size(); i++){
		std::string block = tmp[i];
		resp->push_back(block);
	}
	return 0;
}


#include "proc_kv.cpp"
#include "proc_hash.cpp"
#include "proc_zset.cpp"
