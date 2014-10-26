#include "version.h"
#include "util/log.h"
#include "util/strings.h"
#include "serv.h"
#include "net/proc.h"
#include "net/server.h"

SSDBServer::SSDBServer(SSDB *ssdb, const Config &conf){
	this->ssdb = ssdb;
	backend_dump = new BackendDump(ssdb);
	backend_sync = new BackendSync(ssdb);
	expiration = new ExpirationHandler(ssdb);

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
				Slave *slave = new Slave(ssdb, ssdb->meta_db, ip.c_str(), port, is_mirror);
				if(!id.empty()){
					slave->set_id(id);
				}
				slave->start();
				slaves.push_back(slave);
			}
		}
	}
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

	log_debug("SSDBServer finalized");
}
		
int proc_clear_binlog(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	serv->ssdb->binlogs->flush();
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

int proc_key_range(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
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

int proc_get_key_range(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	resp->push_back("ok");
	std::string s, e;
	int ret = serv->ssdb->get_kv_range(&s, &e);
	if(ret == -1){
		resp->push_back("ok");
	}else{
		resp->push_back(s);
		resp->push_back(e);
	}
	return 0;
}

int proc_set_key_range(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	if(req.size() != 3){
		resp->push_back("client_error");
	}else{
		serv->ssdb->set_kv_range(req[1].String(), req[2].String());
		resp->push_back("ok");
	}
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
