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

static int QFRONT = 2;
static int QBACK  = 3;

static inline
int proc_qpush_func(Server *serv, Link *link, const Request &req, Response *resp, int front_or_back){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		int64_t size = 0;
		std::vector<Bytes>::const_iterator it;
		it = req.begin() + 2;
		for(; it != req.end(); it += 1){
			const Bytes &item = *it;
			if(front_or_back == QFRONT){
				size = serv->ssdb->qpush_front(req[1], item);
			}else{
				size = serv->ssdb->qpush_back(req[1], item);
			}
			if(size == -1){
				resp->push_back("error");
				return 0;
			}
			if(serv->ssdb->qsize_max > 0 && size > serv->ssdb->qsize_max){
				int64_t pop_count = size - serv->ssdb->qsize_max;
				for(int64_t i = 0; i < pop_count; i++) {
					std::string item;
					if(front_or_back == QFRONT){
						serv->ssdb->qpop_back(req[1], &item);
					}else{
						serv->ssdb->qpop_front(req[1], &item);
					}
					size = size - 1;
				}
			}
		}
		
		char buf[20];
		sprintf(buf, "%" PRId64 "", size);
		resp->push_back("ok");
		resp->push_back(buf);
	}
	return 0;
}

static int proc_qpush_front(Server *serv, Link *link, const Request &req, Response *resp){
	return proc_qpush_func(serv, link, req, resp, QFRONT);
}

static int proc_qpush_back(Server *serv, Link *link, const Request &req, Response *resp){
	return proc_qpush_func(serv, link, req, resp, QBACK);
}

static int proc_qpush(Server *serv, Link *link, const Request &req, Response *resp){
	return proc_qpush_func(serv, link, req, resp, QBACK);
}


static inline
int proc_qpop_func(Server *serv, Link *link, const Request &req, Response *resp, int front_or_back){
	if(req.size() < 2){
		resp->push_back("client_error");
		return 0;
	}
	
	int size = 1;
	if(req.size() > 2){
		size = req[2].Int();
		if(size <= 0){
			resp->push_back("error");
			resp->push_back("bad parameter");
			return 0;
		}
	}
		
	int ret;
	std::string item;

	if(size == 1){
		if(front_or_back == QFRONT){
			ret = serv->ssdb->qpop_front(req[1], &item);
		}else{
			ret = serv->ssdb->qpop_back(req[1], &item);
		}
		if(ret == -1){
			resp->push_back("error");
		}else if(ret == 0){
			resp->push_back("not_found");
		}else{
			resp->push_back("ok");
			resp->push_back(item);
		}
	}else{
		resp->push_back("ok");
		while(--size >= 0){
			if(front_or_back == QFRONT){
				ret = serv->ssdb->qpop_front(req[1], &item);
			}else{
				ret = serv->ssdb->qpop_back(req[1], &item);
			}
			if(ret <= 0){
				break;
			}else{
				resp->push_back(item);
			}
		}
	}

	return 0;
}

static int proc_qpop_front(Server *serv, Link *link, const Request &req, Response *resp){
	return proc_qpop_func(serv, link, req, resp, QFRONT);
}

static int proc_qpop_back(Server *serv, Link *link, const Request &req, Response *resp){
	return proc_qpop_func(serv, link, req, resp, QBACK);
}

static int proc_qpop(Server *serv, Link *link, const Request &req, Response *resp){
	return proc_qpop_func(serv, link, req, resp, QFRONT);
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

static int proc_qrlist(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
	}else{
		uint64_t limit = req[3].Uint64();
		std::vector<std::string> list;
		int ret = serv->ssdb->qrlist(req[1], req[2], limit, &list);
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
			int ret = serv->ssdb->qpop_front(req[1], &item);
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

static int proc_qslice(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
	}else{
		int64_t begin = req[2].Int64();
		int64_t end = req[3].Int64();
		std::vector<std::string> list;
		int ret = serv->ssdb->qslice(req[1], begin, end, &list);
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

static int proc_qrange(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
	}else{
		int64_t begin = req[2].Int64();
		int64_t limit = req[3].Uint64();
		int64_t end;
		if(limit >= 0){
			end = begin + limit - 1;
		}else{
			end = -1;
		}
		std::vector<std::string> list;
		int ret = serv->ssdb->qslice(req[1], begin, end, &list);
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

static int proc_qget(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		int64_t index = req[2].Int64();
		std::string item;
		int ret = serv->ssdb->qget(req[1], index, &item);
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
