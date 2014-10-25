#include "proc.h"
#include "serv.h"

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

#define DEF_PROC(f) int proc_##f(Server *serv, Link *link, const Request &req, Response *resp)
	DEF_PROC(get);
	DEF_PROC(set);
	DEF_PROC(setx);
	DEF_PROC(setnx);
	DEF_PROC(getset);
	DEF_PROC(getbit);
	DEF_PROC(setbit);
	DEF_PROC(countbit);
	DEF_PROC(substr);
	DEF_PROC(getrange);
	DEF_PROC(strlen);
	DEF_PROC(redis_bitcount);
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
	DEF_PROC(hrlist);
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
	DEF_PROC(zrlist);
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
	DEF_PROC(qtrim_front);
	DEF_PROC(qtrim_back);
	DEF_PROC(qfix);
	DEF_PROC(qclear);
	DEF_PROC(qlist);
	DEF_PROC(qrlist);
	DEF_PROC(qslice);
	DEF_PROC(qrange);
	DEF_PROC(qget);
	DEF_PROC(qset);

	DEF_PROC(dump);
	DEF_PROC(sync140);
	DEF_PROC(info);
	DEF_PROC(compact);
	DEF_PROC(key_range);
	DEF_PROC(get_key_range);
	DEF_PROC(set_key_range);
	DEF_PROC(ttl);
	DEF_PROC(expire);
	DEF_PROC(clear_binlog);
	DEF_PROC(ping);
	DEF_PROC(auth);
#undef DEF_PROC


#define PROC(c, f)     {#c, f, 0, proc_##c, 0, 0, 0, 0}
#define PROC_KP1(c, f) {#c, f, 0, proc_##c, 0, 0, 0, 1}

static Command commands[] = {
	PROC_KP1(get, "r"),
	PROC_KP1(set, "wt"),
	PROC_KP1(del, "wt"),
	PROC_KP1(setx, "wt"),
	PROC_KP1(setnx, "wt"),
	PROC_KP1(getset, "wt"),
	PROC_KP1(getbit, "r"),
	PROC_KP1(setbit, "wt"),
	PROC_KP1(countbit, "r"),
	PROC_KP1(substr, "r"),
	PROC_KP1(getrange, "r"),
	PROC_KP1(strlen, "r"),
	PROC_KP1(redis_bitcount, "r"),
	PROC_KP1(incr, "wt"),
	PROC_KP1(decr, "wt"),
	PROC(scan, "rt"),
	PROC(rscan, "rt"),
	PROC(keys, "rt"),
	PROC(exists, "r"),
	PROC(multi_exists, "r"),
	PROC(multi_get, "rt"),
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
	PROC(hrlist, "rt"),
	PROC(hexists, "r"),
	PROC(multi_hexists, "r"),
	PROC(multi_hsize, "r"),
	PROC(multi_hget, "rt"),
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
	PROC(zrlist, "rt"),
	PROC(zcount, "rt"),
	PROC(zsum, "rt"),
	PROC(zavg, "rt"),
	PROC(zremrangebyrank, "wt"),
	PROC(zremrangebyscore, "wt"),
	PROC(zexists, "r"),
	PROC(multi_zexists, "r"),
	PROC(multi_zsize, "r"),
	PROC(multi_zget, "rt"),
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
	PROC(qtrim_front, "wt"),
	PROC(qtrim_back, "wt"),
	PROC(qfix, "wt"),
	PROC(qclear, "wt"),
	PROC(qlist, "rt"),
	PROC(qrlist, "rt"),
	PROC(qslice, "rt"),
	PROC(qrange, "rt"),
	PROC(qget, "r"),
	PROC(qset, "wt"),

	PROC(clear_binlog, "wt"),

	PROC(dump, "b"),
	PROC(sync140, "b"),
	PROC(info, "r"),
	// doing compaction in a reader thread, because we have only one
	// writer thread(for performance reason), we don't want to block writes
	PROC(compact, "rt"),
	PROC(key_range, "r"), // deprecated
	//
	PROC(get_key_range, "r"),
	// set_key_range must run in the main thread
	PROC(set_key_range, "r"),

	PROC(ttl, "r"),
	PROC(expire, "wt"),
	PROC(ping, "r"),
	PROC(auth, "r"),

	{NULL, NULL, 0, NULL}
};
#undef PROC


static proc_map_t proc_map;

ProcMap::ProcMap(){
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
}

ProcMap::~ProcMap(){
}

Command* ProcMap::find(const Bytes &str){
	proc_map_t::iterator it = proc_map.find(str);
	if(it != proc_map.end()){
		return it->second;
	}
	return NULL;
}

int proc_info(Server *serv, Link *link, const Request &req, Response *resp){
	resp->push_back("ok");
	resp->push_back("ssdb-server");
	resp->push_back("version");
	resp->push_back(SSDB_VERSION);
	{
		resp->push_back("links");
		resp->add(serv->link_count);
	}
	{
		int64_t calls = 0;
		for(Command *cmd=commands; cmd->name; cmd++){
			calls += cmd->calls;
		}
		resp->push_back("total_calls");
		resp->add(calls);
	}

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

	{
		std::string s = serv->ssdb->binlogs->stats();
		resp->push_back("replication");
		resp->push_back(s);
	}
	std::vector<Slave *>::iterator it;
	for(it = serv->slaves.begin(); it != serv->slaves.end(); it++){
		Slave *slave = *it;
		std::string s = slave->stats();
		resp->push_back("replication");
		resp->push_back(s);
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

