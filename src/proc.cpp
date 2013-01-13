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

CommandProc::CommandProc(const SSDB *ssdb){
	this->ssdb = ssdb;
	backend_dump = new BackendDump(ssdb);
	backend_sync = new BackendSync(ssdb);

#define PROC(c) proc_map[#c] = &CommandProc::proc_##c
	PROC(get);
	PROC(multi_get);
	PROC(set);
	PROC(multi_set);
	PROC(del);
	PROC(multi_del);
	PROC(scan);
	PROC(keys);
	PROC(incr);
	PROC(decr);

	PROC(zget);
	PROC(zset);
	PROC(zdel);
	PROC(zscan);
	PROC(zkeys);
	PROC(zincr);
	PROC(zdecr);

	PROC(hget);
	PROC(hset);
	PROC(hdel);
	PROC(hscan);
	PROC(hkeys);
	PROC(hincr);
	PROC(hdecr);

	PROC(dump);
	PROC(sync);
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
	if(log_level() >= Logger::LEVEL_DEBUG){
		std::string log_buf = serialize_req(req);
		log_debug("req: %s", log_buf.c_str());
	}

	int ret = 0;
	proc_map_t::iterator it = proc_map.find(req[0]);
	if(it == proc_map.end()){
		resp->push_back("client_error");
		resp->push_back("Unknown Command: " + req[0].String());
	}else{
		proc_t p = it->second;
		ret = (this->*p)(link, req, resp);
	}

	if(log_level() >= Logger::LEVEL_DEBUG){
		std::string log_buf = serialize_req(*resp);
		log_debug("resp: %s", log_buf.c_str());
	}

	return ret;
}

int CommandProc::proc_set(const Link &link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		int ret = ssdb->set(req[1], req[2]);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
		}
	}
	return 0;
}

int CommandProc::proc_multi_set(const Link &link, const Request &req, Response *resp){
	if(req.size() < 3 || req.size() % 2 != 1){
		resp->push_back("client_error");
	}else{
		int ret = 0;
		// TODO: write batch
		for(int i=1; i < req.size(); i += 2){
			ret = ssdb->set(req[i], req[i+1]);
			if(ret == -1){
				break;
			}
		}
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
		}
	}
	return 0;
}

int CommandProc::proc_get(const Link &link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		std::string val;
		int ret = ssdb->get(req[1], &val);
		if(ret == 1){
			resp->push_back("ok");
			resp->push_back(val);
		}else if(ret == 0){
			resp->push_back("not_found");
		}else{
			log_error("fail");
			resp->push_back("fail");
		}
	}
	return 0;
}

int CommandProc::proc_multi_get(const Link &link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		// TODO: write batch
		resp->push_back("ok");
		for(int i=1; i<req.size(); i++){
			std::string val;
			int ret = ssdb->get(req[i], &val);
			if(ret == 1){
				resp->push_back(req[i].String());
				resp->push_back(val);
			}else if(ret == 0){
				// not found
			}else{
				// error
				log_error("fail");
			}
		}
	}
	return 0;
}

int CommandProc::proc_del(const Link &link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		int ret = ssdb->del(req[1]);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
		}
	}
	return 0;
}

int CommandProc::proc_multi_del(const Link &link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		// TODO: write batch
		resp->push_back("ok");
		for(int i=1; i<req.size(); i++){
			int ret = ssdb->del(req[i]);
			if(ret == -1){
				//
			}else{
				//
			}
		}
	}
	return 0;
}

int CommandProc::proc_scan(const Link &link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
	}else{
		int limit = req[3].Int();
		KIterator *it = ssdb->scan(req[1], req[2], limit);
		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
			resp->push_back(it->val);
		}
		delete it;
	}
	return 0;
}

int CommandProc::proc_keys(const Link &link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
	}else{
		int limit = req[3].Int();
		KIterator *it = ssdb->scan(req[1], req[2], limit);
		it->return_val(false);

		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
		}
		delete it;
	}
	return 0;
}

// dir := +1|-1
static int _incr(const SSDB *ssdb, const Request &req, Response *resp, int dir){
	if(req.size() <= 1){
		resp->push_back("client_error");
	}else{
		std::string new_val;
		int64_t val = 1;
		if(req.size() > 2){
			val = req[2].Int64();
		}
		int ret = ssdb->incr(req[1], dir * val, &new_val);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
			resp->push_back(new_val);
		}
	}
	return 0;
}

int CommandProc::proc_incr(const Link &link, const Request &req, Response *resp){
	return _incr(ssdb, req, resp, 1);
}

int CommandProc::proc_decr(const Link &link, const Request &req, Response *resp){
	return _incr(ssdb, req, resp, -1);
}


/* zset */

int CommandProc::proc_zset(const Link &link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
	}else{
		int ret = ssdb->zset(req[1], req[2], req[3]);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
		}
	}
	return 0;
}

int CommandProc::proc_zget(const Link &link, const Request &req, Response *resp){
	if(req.size() >= 3){
		std::string score;
		int ret = ssdb->zget(req[1], req[2], &score);
		if(ret == 1){
			resp->push_back("ok");
			resp->push_back(score);
		}else if(ret == 0){
			resp->push_back("not_found");
		}else{
			log_error("fail");
			resp->push_back("fail");
		}
	}else{
		resp->push_back("client_error");
	}
	return 0;
}

int CommandProc::proc_zdel(const Link &link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		int ret = ssdb->zdel(req[1], req[2]);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
		}
	}
	return 0;
}

int CommandProc::proc_zscan(const Link &link, const Request &req, Response *resp){
	if(req.size() < 6){
		resp->push_back("client_error");
	}else{
		int limit = req[5].Int();
		ZIterator *it = ssdb->zscan(req[1], req[2], req[3], req[4], limit);
		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
			resp->push_back(it->score);
		}
		delete it;
	}
	return 0;
}

int CommandProc::proc_zkeys(const Link &link, const Request &req, Response *resp){
	if(req.size() < 6){
		resp->push_back("client_error");
	}else{
		int limit = req[5].Int();
		ZIterator *it = ssdb->zscan(req[1], req[2], req[3], req[4], limit);
		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
		}
		delete it;
	}
	return 0;
}

// dir := +1|-1
static int _zincr(const SSDB *ssdb, const Request &req, Response *resp, int dir){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		std::string new_val;
		int64_t val = 1;
		if(req.size() > 3){
			val = req[3].Int64();
		}
		int ret = ssdb->zincr(req[1], req[2], dir * val, &new_val);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
			resp->push_back(new_val);
		}
	}
	return 0;
}

int CommandProc::proc_zincr(const Link &link, const Request &req, Response *resp){
	return _zincr(ssdb, req, resp, 1);
}

int CommandProc::proc_zdecr(const Link &link, const Request &req, Response *resp){
	return _zincr(ssdb, req, resp, -1);
}


/* hash */

int CommandProc::proc_hset(const Link &link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		int ret = ssdb->hset(req[1], req[2], req[3]);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
		}
	}
	return 0;
}

int CommandProc::proc_hget(const Link &link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		std::string val;
		int ret = ssdb->hget(req[1], req[2], &val);
		if(ret == 1){
			resp->push_back("ok");
			resp->push_back(val);
		}else if(ret == 0){
			resp->push_back("not_found");
		}else{
			log_error("fail");
			resp->push_back("fail");
		}
	}
	return 0;
}

int CommandProc::proc_hdel(const Link &link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		int ret = ssdb->hdel(req[1], req[2]);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
		}
	}
	return 0;
}

int CommandProc::proc_hscan(const Link &link, const Request &req, Response *resp){
	if(req.size() < 5){
		resp->push_back("client_error");
	}else{
		int limit = req[4].Int();
		HIterator *it = ssdb->hscan(req[1], req[2], req[3], limit);
		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
			resp->push_back(it->val);
		}
		delete it;
	}
	return 0;
}

int CommandProc::proc_hkeys(const Link &link, const Request &req, Response *resp){
	if(req.size() < 5){
		resp->push_back("client_error");
	}else{
		int limit = req[4].Int();
		HIterator *it = ssdb->hscan(req[1], req[2], req[3], limit);
		it->return_val(false);

		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
		}
		delete it;
	}
	return 0;
}

// dir := +1|-1
static int _hincr(const SSDB *ssdb, const Request &req, Response *resp, int dir){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		std::string new_val;
		int64_t val = 1;
		if(req.size() > 3){
			val = req[3].Int64();
		}
		int ret = ssdb->hincr(req[1], req[2], dir * val, &new_val);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
			resp->push_back(new_val);
		}
	}
	return 0;
}

int CommandProc::proc_hincr(const Link &link, const Request &req, Response *resp){
	return _hincr(ssdb, req, resp, 1);
}

int CommandProc::proc_hdecr(const Link &link, const Request &req, Response *resp){
	return _hincr(ssdb, req, resp, -1);
}


/*****/

int CommandProc::proc_dump(const Link &link, const Request &req, Response *resp){
	backend_dump->proc(&link);
	return PROC_BACKEND;
}

int CommandProc::proc_sync(const Link &link, const Request &req, Response *resp){
	backend_sync->proc(&link);
	return PROC_BACKEND;
}
