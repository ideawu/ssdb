/*
Copyright (c) 2012-2018 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "proc_sys.h"

int proc_flushdb(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	if(serv->slaves.size() > 0 || serv->backend_sync->stats().size() > 0){
		resp->push_back("error");
		resp->push_back("flushdb is not allowed when replication is in use!");
		return 0;
	}
	serv->ssdb->flushdb();
	resp->push_back("ok");
	return 0;
}

int proc_compact(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	serv->ssdb->compact();
	resp->push_back("ok");
	return 0;
}

int proc_ignore_key_range(NetworkServer *net, Link *link, const Request &req, Response *resp){
	link->ignore_key_range = true;
	resp->push_back("ok");
	return 0;
}

// get kv_range, hash_range, zset_range, list_range
int proc_get_key_range(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	std::string s, e;
	int ret = serv->get_kv_range(&s, &e);
	if(ret == -1){
		resp->push_back("error");
	}else{
		resp->push_back("ok");
		resp->push_back(s);
		resp->push_back(e);
		// TODO: hash_range, zset_range, list_range
	}
	return 0;
}

int proc_get_kv_range(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	std::string s, e;
	int ret = serv->get_kv_range(&s, &e);
	if(ret == -1){
		resp->push_back("error");
	}else{
		resp->push_back("ok");
		resp->push_back(s);
		resp->push_back(e);
	}
	return 0;
}

int proc_set_kv_range(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	if(req.size() != 3){
		resp->push_back("client_error");
	}else{
		serv->set_kv_range(req[1].String(), req[2].String());
		resp->push_back("ok");
	}
	return 0;
}

int proc_dbsize(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	uint64_t size = serv->ssdb->size();
	resp->push_back("ok");
	resp->push_back(str(size));
	return 0;
}

int proc_version(NetworkServer *net, Link *link, const Request &req, Response *resp){
	resp->push_back("ok");
	resp->push_back(SSDB_VERSION);
	return 0;
}

int proc_info(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	resp->push_back("ok");
	resp->push_back("ssdb-server");
	resp->push_back("version");
	resp->push_back(SSDB_VERSION);
	{
		resp->push_back("links");
		resp->add(net->link_count);
	}
	{
		int64_t calls = 0;
		proc_map_t::iterator it;
		for(it=net->proc_map.begin(); it!=net->proc_map.end(); it++){
			Command *cmd = it->second;
			calls += cmd->calls;
		}
		resp->push_back("total_calls");
		resp->add(calls);
	}
	
	{
		uint64_t size = serv->ssdb->size();
		resp->push_back("dbsize");
		resp->push_back(str(size));
	}

	{
		std::string s = serv->ssdb->binlogs->stats();
		resp->push_back("binlogs");
		resp->push_back(s);
	}
	{
		std::vector<std::string> syncs = serv->backend_sync->stats();
		std::vector<std::string>::iterator it;
		for(it = syncs.begin(); it != syncs.end(); it++){
			std::string s = *it;
			resp->push_back("replication");
			resp->push_back(s);
		}
	}
	{
		std::vector<Slave *>::iterator it;
		for(it = serv->slaves.begin(); it != serv->slaves.end(); it++){
			Slave *slave = *it;
			std::string s = slave->stats();
			resp->push_back("replication");
			resp->push_back(s);
		}
	}
	{
		std::string val;
		std::string s, e;
		serv->get_kv_range(&s, &e);
		char buf[512];
		{
			snprintf(buf, sizeof(buf), "    kv  : \"%s\" - \"%s\"",
				str_escape(s).c_str(),
				str_escape(e).c_str()
				);
			val.append(buf);
		}
		{
			snprintf(buf, sizeof(buf), "\n    hash: \"\" - \"\"");
			val.append(buf);
		}
		{
			snprintf(buf, sizeof(buf), "\n    zset: \"\" - \"\"");
			val.append(buf);
		}
		{
			snprintf(buf, sizeof(buf), "\n    list: \"\" - \"\"");
			val.append(buf);
		}
		resp->push_back("serv_key_range");
		resp->push_back(val);
	}

	if(req.size() == 1 || req[1] == "range"){
		std::string val;
		std::vector<std::string> tmp;
		int ret = serv->ssdb->key_range(&tmp);
		if(ret == 0){
			char buf[512];
			
			snprintf(buf, sizeof(buf), "    kv  : \"%s\" - \"%s\"",
				hexmem(tmp[0].data(), tmp[0].size()).c_str(),
				hexmem(tmp[1].data(), tmp[1].size()).c_str()
				);
			val.append(buf);
			
			snprintf(buf, sizeof(buf), "\n    hash: \"%s\" - \"%s\"",
				hexmem(tmp[2].data(), tmp[2].size()).c_str(),
				hexmem(tmp[3].data(), tmp[3].size()).c_str()
				);
			val.append(buf);
			
			snprintf(buf, sizeof(buf), "\n    zset: \"%s\" - \"%s\"",
				hexmem(tmp[4].data(), tmp[4].size()).c_str(),
				hexmem(tmp[5].data(), tmp[5].size()).c_str()
				);
			val.append(buf);
			
			snprintf(buf, sizeof(buf), "\n    list: \"%s\" - \"%s\"",
				hexmem(tmp[6].data(), tmp[6].size()).c_str(),
				hexmem(tmp[7].data(), tmp[7].size()).c_str()
				);
			val.append(buf);
		}
		resp->push_back("data_key_range");
		resp->push_back(val);
	}

	if(req.size() == 1 || req[1] == "leveldb"){
		std::vector<std::string> tmp = serv->ssdb->info();
		for(int i=0; i<(int)tmp.size(); i++){
			std::string block = tmp[i];
			resp->push_back(block);
		}
	}

	if(req.size() > 1 && req[1] == "cmd"){
		proc_map_t::iterator it;
		for(it=net->proc_map.begin(); it!=net->proc_map.end(); it++){
			Command *cmd = it->second;
			resp->push_back("cmd." + cmd->name);
			char buf[128];
			snprintf(buf, sizeof(buf), "calls: %" PRIu64 "\ttime_wait: %.0f\ttime_proc: %.0f",
				cmd->calls, cmd->time_wait, cmd->time_proc);
			resp->push_back(buf);
		}
	}
	
	return 0;
}

int proc_dump(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	serv->backend_dump->proc(link);
	return PROC_BACKEND;
}

int proc_sync140(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	serv->backend_sync->proc(link);
	return PROC_BACKEND;
}

// slaveof id host port [auth last_seq last_key]
int proc_slaveof(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);
	
	std::string id = req[1].String();
	std::string host = req[2].String();
	int port = req[3].Int();
	std::string auth;
	uint64_t last_seq = 0;
	std::string last_key;
	log_info("start slaveof: %s:%d, type: sync", host.c_str(), port);
	if(req.size() > 4 && !req[4].empty()){
		auth = req[4].String();
		log_info("    auth: ***");
	}
	if(req.size() > 5 && !req[5].empty()){
		last_seq = req[5].Uint64();
		log_info("    last_seq: %" PRIu64, last_seq);
	}
	if(req.size() > 6 && !req[6].empty()){
		last_key = req[6].String();
		log_info("    last_key: %s", hexmem(last_key.data(), last_key.size()).c_str());
	}
	
	serv->slaveof(id, host, port, auth, last_seq, last_key, false, 0);

	resp->push_back("ok");
	return 0;
}

int proc_clear_binlog(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	serv->ssdb->binlogs->flush();
	resp->push_back("ok");
	return 0;
}
