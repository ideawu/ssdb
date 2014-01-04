/* queue */

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

static int proc_qfront(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		std::string item;
		int ret = serv->ssdb->qfront(req[1], &item);
		if(ret == -1){
			resp->push_back("error");
		}else if(ret == 0){
			resp->push_back("not_found");
		}else{
			resp->push_back("ok");
			resp->push_back(item);
		}
	}
	return 0;
}

static int proc_qback(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		std::string item;
		int ret = serv->ssdb->qback(req[1], &item);
		if(ret == -1){
			resp->push_back("error");
		}else if(ret == 0){
			resp->push_back("not_found");
		}else{
			resp->push_back("ok");
			resp->push_back(item);
		}
	}
	return 0;
}

static int proc_qpush(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		int ret = serv->ssdb->qpush(req[1], req[2]);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
		}
	}
	return 0;
}

static int proc_qpop(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		std::string item;
		int ret = serv->ssdb->qpop(req[1], &item);
		if(ret == -1){
			resp->push_back("error");
		}else if(ret == 0){
			resp->push_back("not_found");
		}else{
			resp->push_back("ok");
			resp->push_back(item);
		}
	}
	return 0;
}

static int proc_qlist(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
	}else{
		uint64_t limit = req[3].Uint64();
		std::vector<std::string> list;
		int ret = serv->ssdb->qlist(req[1], req[2], limit, &list);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
			for(int i=0; i<list.size(); i++){
				resp->push_back(list[i]);
			}
		}
	}
	return 0;
}

static int proc_qfix(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		int ret = serv->ssdb->qfix(req[1]);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
		}
	}
	return 0;
}

static int proc_qclear(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		uint64_t total = 0;
		
		while(1){
			std::string item;
			int ret = serv->ssdb->qpop(req[1], &item);
			if(ret == 0){
				break;
			}
			if(ret == -1){
				return -1;
			}
			total += 1;
		}
		
		char buf[20];
		snprintf(buf, sizeof(buf), "%" PRIu64 "", total);
		resp->push_back("ok");
		resp->push_back(buf);

	}
	return 0;
}
