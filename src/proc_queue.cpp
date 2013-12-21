/* queue */
//qoffer q va
//qpoll q
//qpeek q
//qsize q
static int proc_qsize(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		int64_t ret = serv->ssdb->qsize(req[1]);
		if(ret == -1){
			resp->push_back("error");
		}else{
			char buf[20];
			sprintf(buf, "%" PRIu64 "", ret);
			resp->push_back("ok");
			resp->push_back(buf);
		}
	}
	return 0;
}

static int proc_qoffer(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		int ret = serv->ssdb->qoffer(req[1], req[2]);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
			resp->push_back("1");
		}
	}
	return 0;
}

static int proc_qpoll(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		std::string val;
		int ret = serv->ssdb->qpoll(req[1], &val);
		if(ret == 1){
			resp->push_back("ok");
			resp->push_back(val);
		}else if(ret == 0){
			resp->push_back("not_found");
		}else{
			log_error("error");
			resp->push_back("error");
		}
	}
	return 0;
}

static int proc_qpeek(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		std::string val;
		int ret = serv->ssdb->qpeek(req[1], &val);
		if(ret == 1){
			resp->push_back("ok");
			resp->push_back(val);
		}else if(ret == 0){
			resp->push_back("not_found");
		}else{
			log_error("error");
			resp->push_back("error");
		}
	}
	return 0;
}


