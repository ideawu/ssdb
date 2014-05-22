#include "version.h"
#include "util/log.h"
#include "util/strings.h"
#include "serv.h"
#include "t_kv.h"
#include "t_hash.h"
#include "t_zset.h"

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


#define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
#if GCC_VERSION >= 403
	#include <tr1/unordered_map>
	typedef std::tr1::unordered_map<Bytes, Command *, BytesHash, BytesEqual> proc_map_t;
#else
	#ifdef NEW_MAC
		#include <unordered_map>
		typedef std::unordered_map<Bytes, Command *, BytesHash, BytesEqual> proc_map_t;
	#else
		#include <ext/hash_map>
		typedef __gnu_cxx::hash_map<Bytes, Command *, BytesHash, BytesEqual> proc_map_t;
	#endif
#endif


static proc_map_t proc_map;

#define DEF_PROC(f) static int proc_##f(Server *serv, Link *link, const Request &req, Response *resp)
	DEF_PROC(get);
	DEF_PROC(set);
	DEF_PROC(setx);
	DEF_PROC(setnx);
	DEF_PROC(getset);
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
	DEF_PROC(hclear);
	DEF_PROC(hgetall);
	DEF_PROC(hscan);
	DEF_PROC(hrscan);
	DEF_PROC(hkeys);
	DEF_PROC(hvals);
	DEF_PROC(hlist);
	DEF_PROC(hexists);
	DEF_PROC(multi_hexists);
	DEF_PROC(multi_hsize);
	DEF_PROC(multi_hget);
	DEF_PROC(multi_hset);
	DEF_PROC(multi_hdel);

	DEF_PROC(zrank);
	DEF_PROC(zrrank);
	DEF_PROC(zrange);
	DEF_PROC(zrrange);
	DEF_PROC(zsize);
	DEF_PROC(zget);
	DEF_PROC(zset);
	DEF_PROC(zdel);
	DEF_PROC(zincr);
	DEF_PROC(zdecr);
	DEF_PROC(zclear);
	DEF_PROC(zscan);
	DEF_PROC(zrscan);
	DEF_PROC(zkeys);
	DEF_PROC(zlist);
	DEF_PROC(zcount);
	DEF_PROC(zsum);
	DEF_PROC(zavg);
	DEF_PROC(zexists);
	DEF_PROC(zremrangebyrank);
	DEF_PROC(zremrangebyscore);
	DEF_PROC(multi_zexists);
	DEF_PROC(multi_zsize);
	DEF_PROC(multi_zget);
	DEF_PROC(multi_zset);
	DEF_PROC(multi_zdel);
	
	DEF_PROC(qsize);
	DEF_PROC(qfront);
	DEF_PROC(qback);
	DEF_PROC(qpush);
	DEF_PROC(qpush_front);
	DEF_PROC(qpush_back);
	DEF_PROC(qpop);
	DEF_PROC(qpop_front);
	DEF_PROC(qpop_back);
	DEF_PROC(qfix);
	DEF_PROC(qclear);
	DEF_PROC(qlist);
	DEF_PROC(qslice);
	DEF_PROC(qrange);
	DEF_PROC(qget);

	DEF_PROC(dump);
	DEF_PROC(sync140);
	DEF_PROC(info);
	DEF_PROC(compact);
	DEF_PROC(key_range);
	DEF_PROC(ttl);
	DEF_PROC(clear_binlog);
	DEF_PROC(ping);
#undef DEF_PROC


#define PROC(c, f) {#c, f, 0, proc_##c, 0, 0, 0}
static Command commands[] = {
	PROC(get, "r"),
	PROC(set, "wt"),
	PROC(setx, "wt"),
	PROC(setnx, "wt"),
	PROC(getset, "wt"),
	PROC(del, "wt"),
	PROC(incr, "wt"),
	PROC(decr, "wt"),
	PROC(scan, "rt"),
	PROC(rscan, "rt"),
	PROC(keys, "rt"),
	PROC(exists, "r"),
	PROC(multi_exists, "r"),
	PROC(multi_get, "r"),
	PROC(multi_set, "wt"),
	PROC(multi_del, "wt"),

	PROC(hsize, "r"),
	PROC(hget, "r"),
	PROC(hset, "wt"),
	PROC(hdel, "wt"),
	PROC(hincr, "wt"),
	PROC(hdecr, "wt"),
	PROC(hclear, "wt"),
	PROC(hgetall, "rt"),
	PROC(hscan, "rt"),
	PROC(hrscan, "rt"),
	PROC(hkeys, "rt"),
	PROC(hvals, "rt"),
	PROC(hlist, "rt"),
	PROC(hexists, "r"),
	PROC(multi_hexists, "r"),
	PROC(multi_hsize, "r"),
	PROC(multi_hget, "r"),
	PROC(multi_hset, "wt"),
	PROC(multi_hdel, "wt"),

	// because zrank may be extremly slow, execute in a seperate thread
	PROC(zrank, "rt"),
	PROC(zrrank, "rt"),
	PROC(zrange, "rt"),
	PROC(zrrange, "rt"),
	PROC(zsize, "r"),
	PROC(zget, "rt"),
	PROC(zset, "wt"),
	PROC(zdel, "wt"),
	PROC(zincr, "wt"),
	PROC(zdecr, "wt"),
	PROC(zclear, "wt"),
	PROC(zscan, "rt"),
	PROC(zrscan, "rt"),
	PROC(zkeys, "rt"),
	PROC(zlist, "rt"),
	PROC(zcount, "rt"),
	PROC(zsum, "rt"),
	PROC(zavg, "rt"),
	PROC(zremrangebyrank, "wt"),
	PROC(zremrangebyscore, "wt"),
	PROC(zexists, "r"),
	PROC(multi_zexists, "r"),
	PROC(multi_zsize, "r"),
	PROC(multi_zget, "r"),
	PROC(multi_zset, "wt"),
	PROC(multi_zdel, "wt"),

	PROC(qsize, "r"),
	PROC(qfront, "r"),
	PROC(qback, "r"),
	PROC(qpush, "wt"),
	PROC(qpush_front, "wt"),
	PROC(qpush_back, "wt"),
	PROC(qpop, "wt"),
	PROC(qpop_front, "wt"),
	PROC(qpop_back, "wt"),
	PROC(qfix, "wt"),
	PROC(qclear, "wt"),
	PROC(qlist, "rt"),
	PROC(qslice, "rt"),
	PROC(qrange, "rt"),
	PROC(qget, "r"),

	PROC(clear_binlog, "wt"),

	PROC(dump, "b"),
	PROC(sync140, "b"),
	PROC(info, "r"),
	// doing compaction in a reader thread, because we have only one
	// writer thread(for performance reason), we don't want to block writes
	PROC(compact, "rt"),
	PROC(key_range, "r"),

	PROC(ttl, "wt"),
	PROC(ping, "r"),

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
				case 't':
					cmd->flags |= Command::FLAG_THREAD;
					break;
			}
		}
		proc_map[cmd->name] = cmd;
	}
	// for k-v data, list === keys
	proc_map["list"] = proc_map["keys"];

	expiration = new ExpirationHandler(ssdb);
	
	writer = new WorkerPool<ProcWorker, ProcJob>("writer");
	writer->start(WRITER_THREADS);
	reader = new WorkerPool<ProcWorker, ProcJob>("reader");
	reader->start(READER_THREADS);
}

Server::~Server(){
	delete backend_dump;
	delete backend_sync;
	
	delete expiration;
	
	writer->stop();
	delete writer;
	reader->stop();
	delete reader;

	log_debug("Server finalized");
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
		if(cmd->flags & Command::FLAG_THREAD){
			if(cmd->flags & Command::FLAG_WRITE){
				job->result = PROC_THREAD;
				writer->push(*job);
				return; /////
			}else if(cmd->flags & Command::FLAG_READ){
				job->result = PROC_THREAD;
				reader->push(*job);
				return; /////
			}else{
				log_error("bad command config: %s", cmd->name);
			}
		}

		proc_t p = cmd->proc;
		job->time_wait = 1000 *(millitime() - job->stime);
		job->result = (*p)(this, job->link, *req, &resp);
		job->time_proc = 1000 *(millitime() - job->stime);
	}
	if(job->result == PROC_BACKEND){
		return;
	}
	
	if(job->link->send(resp) == -1){
		job->result = PROC_ERROR;
	}else{
		if(log_level() >= Logger::LEVEL_DEBUG){
			log_debug("w:%.3f,p:%.3f, req: %s, resp: %s",
				job->time_wait, job->time_proc,
				serialize_req(*req).c_str(),
				serialize_req(resp).c_str());
		}
	}
}


Server::ProcWorker::ProcWorker(const std::string &name){
	this->name = name;
}

void Server::ProcWorker::init(){
	log_debug("%s %d init", this->name.c_str(), this->id);
}

int Server::ProcWorker::proc(ProcJob *job){
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
		log_debug("w:%.3f,p:%.3f, req: %s, resp: %s",
			job->time_wait, job->time_proc,
			serialize_req(*req).c_str(),
			serialize_req(resp).c_str());
	}
	return 0;
}


static int proc_clear_binlog(Server *serv, Link *link, const Request &req, Response *resp){
	serv->ssdb->binlogs->flush();
	resp->push_back("ok");
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
	
	if(req.size() > 1 && req[1] == "cmd"){
		for(Command *cmd=commands; cmd->name; cmd++){
			char buf[128];
			snprintf(buf, sizeof(buf), "cmd.%s", cmd->name);
			resp->push_back(buf);
			snprintf(buf, sizeof(buf), "calls: %" PRIu64 "\ttime_wait: %.0f\ttime_proc: %.0f",
				cmd->calls, cmd->time_wait, cmd->time_proc);
			resp->push_back(buf);
		}
	}

	if(req.size() == 1 || req[1] == "range"){
		std::vector<std::string> tmp;
		int ret = serv->ssdb->key_range(&tmp);
		if(ret == 0){
			char buf[512];
			
			resp->push_back("key_range.kv");
			snprintf(buf, sizeof(buf), "\"%s\" - \"%s\"",
				hexmem(tmp[0].data(), tmp[0].size()).c_str(),
				hexmem(tmp[1].data(), tmp[1].size()).c_str()
				);
			resp->push_back(buf);
			
			resp->push_back("key_range.hash");
			snprintf(buf, sizeof(buf), "\"%s\" - \"%s\"",
				hexmem(tmp[2].data(), tmp[2].size()).c_str(),
				hexmem(tmp[3].data(), tmp[3].size()).c_str()
				);
			resp->push_back(buf);
			
			resp->push_back("key_range.zset");
			snprintf(buf, sizeof(buf), "\"%s\" - \"%s\"",
				hexmem(tmp[4].data(), tmp[4].size()).c_str(),
				hexmem(tmp[5].data(), tmp[5].size()).c_str()
				);
			resp->push_back(buf);
			
			resp->push_back("key_range.list");
			snprintf(buf, sizeof(buf), "\"%s\" - \"%s\"",
				hexmem(tmp[6].data(), tmp[6].size()).c_str(),
				hexmem(tmp[7].data(), tmp[7].size()).c_str()
				);
			resp->push_back(buf);
		}
	}

	if(req.size() == 1 || req[1] == "leveldb"){
		std::vector<std::string> tmp = serv->ssdb->info();
		for(int i=0; i<(int)tmp.size(); i++){
			std::string block = tmp[i];
			resp->push_back(block);
		}
	}
	
	return 0;
}

static int proc_compact(Server *serv, Link *link, const Request &req, Response *resp){
	serv->ssdb->compact();
	resp->push_back("ok");
	return 0;
}

static int proc_key_range(Server *serv, Link *link, const Request &req, Response *resp){
	std::vector<std::string> tmp;
	int ret = serv->ssdb->key_range(&tmp);
	if(ret == -1){
		resp->push_back("error");
		return -1;
	}
	
	resp->push_back("ok");
	for(int i=0; i<(int)tmp.size(); i++){
		std::string block = tmp[i];
		resp->push_back(block);
	}
	
	return 0;
}

static int proc_ttl(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() == 1 || (req.size() - 1) % 2 != 0){
		resp->push_back("client_error");
	}else{
		for(int i=1; i<req.size(); i+=2){
			serv->expiration->set_ttl(req[i], req[i+1].Int());
		}
		resp->push_back("ok");
	}
	return 0;
}

static int proc_ping(Server *serv, Link *link, const Request &req, Response *resp){
	resp->push_back("ok");
	return 0;
}

#include "proc_kv.cpp"
#include "proc_hash.cpp"
#include "proc_zset.cpp"
#include "proc_queue.cpp"
