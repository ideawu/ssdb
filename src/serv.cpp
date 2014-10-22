#include "version.h"
#include "util/log.h"
#include "util/strings.h"
#include "serv.h"

Server::Server(SSDB *ssdb){
	this->ssdb = ssdb;
	this->link_count = 0;
	this->need_auth = false;
	backend_dump = new BackendDump(ssdb);
	backend_sync = new BackendSync(ssdb);
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

	do{
		// AUTH
		if(this->need_auth && job->link->auth == false && req->at(0) != "auth"){
			resp.push_back("noauth");
			resp.push_back("authentication required");
			break;
		}
		
		Command *cmd = proc_map.find(req->at(0));
		if(!cmd){
			resp.push_back("client_error");
			resp.push_back("Unknown Command: " + req->at(0).String());
			break;
		}
		job->cmd = cmd;
		
		// KEY RANGE
		if(cmd->key_pos > 0){
			const Bytes &key = req->at(cmd->key_pos);
			if((this->kv_range_s.size() && this->kv_range_s >= key)
				|| (this->kv_range_e.size() && this->kv_range_e < key))
			{
				resp.push_back("out_of_range");
				break;
			}
		}
		
		if(cmd->flags & Command::FLAG_THREAD){
			if(cmd->flags & Command::FLAG_WRITE){
				job->result = PROC_THREAD;
				writer->push(*job);
			}else{
				job->result = PROC_THREAD;
				reader->push(*job);
			}
			return;
		}

		proc_t p = cmd->proc;
		job->time_wait = 1000 * (millitime() - job->stime);
		job->result = (*p)(this, job->link, *req, &resp);
		job->time_proc = 1000 * (millitime() - job->stime) - job->time_wait;
	}while(0);
	
	if(job->link->send(resp.resp) == -1){
		job->result = PROC_ERROR;
	}else{
		if(log_level() >= Logger::LEVEL_DEBUG){
			log_debug("w:%.3f,p:%.3f, req: %s, resp: %s",
				job->time_wait, job->time_proc,
				serialize_req(*req).c_str(),
				serialize_req(resp.resp).c_str());
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
	
	proc_t p = job->cmd->proc;
	job->time_wait = 1000 * (millitime() - job->stime);
	job->result = (*p)(job->serv, job->link, *req, &resp);
	job->time_proc = 1000 * (millitime() - job->stime) - job->time_wait;

	if(job->link->send(resp.resp) == -1){
		job->result = PROC_ERROR;
	}else{
		log_debug("w:%.3f,p:%.3f, req: %s, resp: %s",
			job->time_wait, job->time_proc,
			serialize_req(*req).c_str(),
			serialize_req(resp.resp).c_str());
	}
	return 0;
}


int proc_clear_binlog(Server *serv, Link *link, const Request &req, Response *resp){
	serv->ssdb->binlogs->flush();
	resp->push_back("ok");
	return 0;
}

int proc_dump(Server *serv, Link *link, const Request &req, Response *resp){
	serv->backend_dump->proc(link);
	return PROC_BACKEND;
}

int proc_sync140(Server *serv, Link *link, const Request &req, Response *resp){
	serv->backend_sync->proc(link);
	return PROC_BACKEND;
}

int proc_compact(Server *serv, Link *link, const Request &req, Response *resp){
	serv->ssdb->compact();
	resp->push_back("ok");
	return 0;
}

int proc_key_range(Server *serv, Link *link, const Request &req, Response *resp){
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

int proc_set_key_range(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() != 3){
		resp->push_back("client_error");
	}else{
		serv->kv_range_s = req[1].String();
		serv->kv_range_e = req[2].String();
		resp->push_back("ok");
	}
	return 0;
}

int proc_ping(Server *serv, Link *link, const Request &req, Response *resp){
	resp->push_back("ok");
	return 0;
}

int proc_auth(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() != 2){
		resp->push_back("client_error");
	}else{
		if(!serv->need_auth || req[1] == serv->password){
			link->auth = true;
			resp->push_back("ok");
			resp->push_back("1");
		}else{
			resp->push_back("error");
			resp->push_back("invalid password");
		}
	}
	return 0;
}
