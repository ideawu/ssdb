/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "version.h"
#include "util/log.h"
#include "util/strings.h"
#include "serv.h"
#include "net/proc.h"
#include "net/server.h"

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
DEF_PROC(bitcount);
DEF_PROC(del);
DEF_PROC(incr);
DEF_PROC(decr);
DEF_PROC(scan);
DEF_PROC(rscan);
DEF_PROC(keys);
DEF_PROC(rkeys);
DEF_PROC(exists);
DEF_PROC(multi_exists);
DEF_PROC(multi_get);
DEF_PROC(multi_set);
DEF_PROC(multi_del);
DEF_PROC(ttl);
DEF_PROC(expire);

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
DEF_PROC(zfix);
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
DEF_PROC(zpop_front);
DEF_PROC(zpop_back);
	
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
DEF_PROC(version);
DEF_PROC(dbsize);
DEF_PROC(compact);
DEF_PROC(clear_binlog);
DEF_PROC(flushdb);

DEF_PROC(get_key_range);
DEF_PROC(ignore_key_range);
DEF_PROC(get_kv_range);
DEF_PROC(set_kv_range);

DEF_PROC(cluster_add_kv_node);
DEF_PROC(cluster_del_kv_node);
DEF_PROC(cluster_kv_node_list);
DEF_PROC(cluster_set_kv_range);
DEF_PROC(cluster_set_kv_status);
DEF_PROC(cluster_migrate_kv_data);


#define REG_PROC(c, f)     net->proc_map.set_proc(#c, f, proc_##c)

void SSDBServer::reg_procs(NetworkServer *net){
	REG_PROC(get, "rt");
	REG_PROC(set, "wt");
	REG_PROC(del, "wt");
	REG_PROC(setx, "wt");
	REG_PROC(setnx, "wt");
	REG_PROC(getset, "wt");
	REG_PROC(getbit, "rt");
	REG_PROC(setbit, "wt");
	REG_PROC(countbit, "rt");
	REG_PROC(substr, "rt");
	REG_PROC(getrange, "rt");
	REG_PROC(strlen, "rt");
	REG_PROC(bitcount, "rt");
	REG_PROC(incr, "wt");
	REG_PROC(decr, "wt");
	REG_PROC(scan, "rt");
	REG_PROC(rscan, "rt");
	REG_PROC(keys, "rt");
	REG_PROC(rkeys, "rt");
	REG_PROC(exists, "rt");
	REG_PROC(multi_exists, "rt");
	REG_PROC(multi_get, "rt");
	REG_PROC(multi_set, "wt");
	REG_PROC(multi_del, "wt");
	REG_PROC(ttl, "rt");
	REG_PROC(expire, "wt");

	REG_PROC(hsize, "rt");
	REG_PROC(hget, "rt");
	REG_PROC(hset, "wt");
	REG_PROC(hdel, "wt");
	REG_PROC(hincr, "wt");
	REG_PROC(hdecr, "wt");
	REG_PROC(hclear, "wt");
	REG_PROC(hgetall, "rt");
	REG_PROC(hscan, "rt");
	REG_PROC(hrscan, "rt");
	REG_PROC(hkeys, "rt");
	REG_PROC(hvals, "rt");
	REG_PROC(hlist, "rt");
	REG_PROC(hrlist, "rt");
	REG_PROC(hexists, "rt");
	REG_PROC(multi_hexists, "rt");
	REG_PROC(multi_hsize, "rt");
	REG_PROC(multi_hget, "rt");
	REG_PROC(multi_hset, "wt");
	REG_PROC(multi_hdel, "wt");

	// because zrank may be extremly slow, execute in a seperate thread
	REG_PROC(zrank, "rt");
	REG_PROC(zrrank, "rt");
	REG_PROC(zrange, "rt");
	REG_PROC(zrrange, "rt");
	REG_PROC(zsize, "rt");
	REG_PROC(zget, "rt");
	REG_PROC(zset, "wt");
	REG_PROC(zdel, "wt");
	REG_PROC(zincr, "wt");
	REG_PROC(zdecr, "wt");
	REG_PROC(zclear, "wt");
	REG_PROC(zfix, "wt");
	REG_PROC(zscan, "rt");
	REG_PROC(zrscan, "rt");
	REG_PROC(zkeys, "rt");
	REG_PROC(zlist, "rt");
	REG_PROC(zrlist, "rt");
	REG_PROC(zcount, "rt");
	REG_PROC(zsum, "rt");
	REG_PROC(zavg, "rt");
	REG_PROC(zremrangebyrank, "wt");
	REG_PROC(zremrangebyscore, "wt");
	REG_PROC(zexists, "rt");
	REG_PROC(multi_zexists, "rt");
	REG_PROC(multi_zsize, "rt");
	REG_PROC(multi_zget, "rt");
	REG_PROC(multi_zset, "wt");
	REG_PROC(multi_zdel, "wt");
	REG_PROC(zpop_front, "wt");
	REG_PROC(zpop_back, "wt");

	REG_PROC(qsize, "rt");
	REG_PROC(qfront, "rt");
	REG_PROC(qback, "rt");
	REG_PROC(qpush, "wt");
	REG_PROC(qpush_front, "wt");
	REG_PROC(qpush_back, "wt");
	REG_PROC(qpop, "wt");
	REG_PROC(qpop_front, "wt");
	REG_PROC(qpop_back, "wt");
	REG_PROC(qtrim_front, "wt");
	REG_PROC(qtrim_back, "wt");
	REG_PROC(qfix, "wt");
	REG_PROC(qclear, "wt");
	REG_PROC(qlist, "rt");
	REG_PROC(qrlist, "rt");
	REG_PROC(qslice, "rt");
	REG_PROC(qrange, "rt");
	REG_PROC(qget, "rt");
	REG_PROC(qset, "wt");

	REG_PROC(clear_binlog, "wt");
	REG_PROC(flushdb, "wt");

	REG_PROC(dump, "b");
	REG_PROC(sync140, "b");
	REG_PROC(info, "r");
	REG_PROC(version, "r");
	REG_PROC(dbsize, "rt");
	// doing compaction in a reader thread, because we have only one
	// writer thread(for performance reason); we don't want to block writes
	REG_PROC(compact, "rt");

	REG_PROC(ignore_key_range, "r");
	REG_PROC(get_key_range, "r");
	REG_PROC(get_kv_range, "r");
	REG_PROC(set_kv_range, "r");

	REG_PROC(cluster_add_kv_node, "r");
	REG_PROC(cluster_del_kv_node, "r");
	REG_PROC(cluster_kv_node_list, "r");
	REG_PROC(cluster_set_kv_range, "r");
	REG_PROC(cluster_set_kv_status, "r");
	REG_PROC(cluster_migrate_kv_data, "r");
}


SSDBServer::SSDBServer(SSDB *ssdb, SSDB *meta, const Config &conf, NetworkServer *net){
	this->ssdb = (SSDBImpl *)ssdb;
	this->meta = meta;

	net->data = this;
	this->reg_procs(net);

	int sync_speed = conf.get_num("replication.sync_speed");

	backend_dump = new BackendDump(this->ssdb);
	backend_sync = new BackendSync(this->ssdb, sync_speed);
	expiration = new ExpirationHandler(this->ssdb);
	
	cluster = new Cluster(this->ssdb);
	if(cluster->init() == -1){
		log_fatal("cluster init failed!");
		exit(1);
	}

	{ // slaves
		const Config *repl_conf = conf.get("replication");
		if(repl_conf != NULL){
			std::vector<Config *> children = repl_conf->children;
			for(std::vector<Config *>::iterator it = children.begin(); it != children.end(); it++){
				Config *c = *it;
				if(c->key != "slaveof"){
					continue;
				}
				std::string ip = c->get_str("ip");
				int port = c->get_num("port");
				if(ip == ""){
					ip = c->get_str("host");
				}
				if(ip == "" || port <= 0 || port > 65535){
					continue;
				}
				bool is_mirror = false;
				std::string type = c->get_str("type");
				if(type == "mirror"){
					is_mirror = true;
				}else{
					type = "sync";
					is_mirror = false;
				}
				
				std::string id = c->get_str("id");
				
				log_info("slaveof: %s:%d, type: %s", ip.c_str(), port, type.c_str());
				Slave *slave = new Slave(ssdb, meta, ip.c_str(), port, is_mirror);
				if(!id.empty()){
					slave->set_id(id);
				}
				slave->auth = c->get_str("auth");
				slave->start();
				slaves.push_back(slave);
			}
		}
	}

	// load kv_range
	int ret = this->get_kv_range(&this->kv_range_s, &this->kv_range_e);
	if(ret == -1){
		log_fatal("load key_range failed!");
		exit(1);
	}
	log_info("key_range.kv: \"%s\", \"%s\"",
		str_escape(this->kv_range_s).c_str(),
		str_escape(this->kv_range_e).c_str()
		);
}

SSDBServer::~SSDBServer(){
	std::vector<Slave *>::iterator it;
	for(it = slaves.begin(); it != slaves.end(); it++){
		Slave *slave = *it;
		slave->stop();
		delete slave;
	}

	delete backend_dump;
	delete backend_sync;
	delete expiration;
	delete cluster;

	log_debug("SSDBServer finalized");
}

int SSDBServer::set_kv_range(const std::string &start, const std::string &end){
	if(meta->hset("key_range", "kv_s", start) == -1){
		return -1;
	}
	if(meta->hset("key_range", "kv_e", end) == -1){
		return -1;
	}

	kv_range_s = start;
	kv_range_e = end;
	return 0;
}

int SSDBServer::get_kv_range(std::string *start, std::string *end){
	if(meta->hget("key_range", "kv_s", start) == -1){
		return -1;
	}
	if(meta->hget("key_range", "kv_e", end) == -1){
		return -1;
	}
	return 0;
}

bool SSDBServer::in_kv_range(const Bytes &key){
	if((this->kv_range_s.size() && this->kv_range_s >= key)
		|| (this->kv_range_e.size() && this->kv_range_e < key))
	{
		return false;
	}
	return true;
}

bool SSDBServer::in_kv_range(const std::string &key){
	if((this->kv_range_s.size() && this->kv_range_s >= key)
		|| (this->kv_range_e.size() && this->kv_range_e < key))
	{
		return false;
	}
	return true;
}

/*********************/

int proc_clear_binlog(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	serv->ssdb->binlogs->flush();
	resp->push_back("ok");
	return 0;
}

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
