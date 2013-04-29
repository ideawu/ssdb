

static int proc_dump(Server *serv, Link *link, const Request &req, Response *resp){
	serv->backend_dump->proc(link);
	return PROC_BACKEND;
}

static int proc_sync(Server *serv, Link *link, const Request &req, Response *resp){
	serv->backend_sync->proc(link);
	return PROC_BACKEND;
}

static int proc_info(Server *serv, Link *link, const Request &req, Response *resp){
	resp->push_back("ok");
	resp->push_back("ssdb-server");
	resp->push_back("version");
	resp->push_back(SSDB_VERSION);
	
	for(Command *cmd=commands; cmd->name; cmd++){
		char buf[64];
		snprintf(buf, sizeof(buf), "cmd.%s", cmd->name);
		resp->push_back(buf);
		snprintf(buf, sizeof(buf), "%llu %.0f %.0f", cmd->calls, cmd->time_wait, cmd->time_proc);
		resp->push_back(buf);
	}

	std::vector<std::string> tmp = serv->ssdb->info();
	for(int i=0; i<(int)tmp.size(); i++){
		std::string block = tmp[i];
		resp->push_back(block);
	}
	return 0;
}
