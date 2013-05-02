/* zset */

static int proc_zexists(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		int64_t ret = serv->ssdb->zsize(req[1]);
		if(ret == -1){
			resp->push_back("error");
			resp->push_back("0");
		}else if(ret == 0){
			resp->push_back("ok");
			resp->push_back("0");
		}else{
			resp->push_back("ok");
			resp->push_back("1");
		}
	}
	return 0;
}

static int proc_multi_zexists(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		resp->push_back("ok");
		for(Request::const_iterator it=req.begin()+1; it!=req.end(); it++){
			const Bytes &key = *it;
			int64_t ret = serv->ssdb->zsize(key);
			resp->push_back(key.String());
			if(ret > 0){
				resp->push_back("1");
			}else{
				resp->push_back("0");
			}
		}
	}
	return 0;
}

static int proc_multi_zsize(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		resp->push_back("ok");
		for(Request::const_iterator it=req.begin()+1; it!=req.end(); it++){
			const Bytes &key = *it;
			int64_t ret = serv->ssdb->zsize(key);
			resp->push_back(key.String());
			if(ret == -1){
				resp->push_back("-1");
			}else{
				char buf[20];
				sprintf(buf, "%lld", ret);
				resp->push_back(buf);
			}
		}
	}
	return 0;
}

static int proc_multi_zset(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4 || req.size() % 2 != 0){
		resp->push_back("client_error");
	}else{
		const Bytes &name = req[1];
		int ret = serv->ssdb->multi_zset(name, req, 2);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
			char buf[20];
			sprintf(buf, "%d", ret);
			resp->push_back(buf);
		}
	}
	return 0;
}

static int proc_multi_zdel(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		const Bytes &name = req[1];
		int ret = serv->ssdb->multi_zdel(name, req, 2);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
			char buf[20];
			sprintf(buf, "%d", ret);
			resp->push_back(buf);
		}
	}
	return 0;
}

static int proc_multi_zget(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		resp->push_back("ok");
		Request::const_iterator it=req.begin() + 1;
		const Bytes name = *it;
		it ++;
		for(; it!=req.end(); it+=1){
			const Bytes &key = *it;
			std::string score;
			int ret = serv->ssdb->zget(name, key, &score);
			if(ret == 1){
				resp->push_back(key.String());
				resp->push_back(score);
			}else if(ret == 0){
				//
			}else{
				log_error("fail");
				resp->push_back("0");
			}
		}
	}
	return 0;
}

static int proc_zset(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
	}else{
		int ret = serv->ssdb->zset(req[1], req[2], req[3]);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
			char buf[20];
			sprintf(buf, "%d", ret);
			resp->push_back(buf);
		}
	}
	return 0;
}

static int proc_zsize(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		int64_t ret = serv->ssdb->zsize(req[1]);
		if(ret == -1){
			resp->push_back("error");
		}else{
			char buf[20];
			sprintf(buf, "%lld", ret);
			resp->push_back("ok");
			resp->push_back(buf);
		}
	}
	return 0;
}

static int proc_zget(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() >= 3){
		std::string score;
		int ret = serv->ssdb->zget(req[1], req[2], &score);
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

static int proc_zdel(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		int ret = serv->ssdb->zdel(req[1], req[2]);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
			if(ret == 0){
				resp->push_back("0");
			}else{
				resp->push_back("1");
			}
		}
	}
	return 0;
}

static int proc_zscan(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 6){
		resp->push_back("client_error");
	}else{
		int limit = req[5].Int();
		ZIterator *it = serv->ssdb->zscan(req[1], req[2], req[3], req[4], limit);
		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
			resp->push_back(it->score);
		}
		delete it;
	}
	return 0;
}

static int proc_zrscan(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 6){
		resp->push_back("client_error");
	}else{
		int limit = req[5].Int();
		ZIterator *it = serv->ssdb->zrscan(req[1], req[2], req[3], req[4], limit);
		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
			resp->push_back(it->score);
		}
		delete it;
	}
	return 0;
}

static int proc_zkeys(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 6){
		resp->push_back("client_error");
	}else{
		int limit = req[5].Int();
		ZIterator *it = serv->ssdb->zscan(req[1], req[2], req[3], req[4], limit);
		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
		}
		delete it;
	}
	return 0;
}

static int proc_zlist(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
	}else{
		int limit = req[3].Int();
		std::vector<std::string> list;
		int ret = serv->ssdb->zlist(req[1], req[2], limit, &list);
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

// dir := +1|-1
static int _zincr(SSDB *ssdb, const Request &req, Response *resp, int dir){
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

static int proc_zincr(Server *serv, Link *link, const Request &req, Response *resp){
	return _zincr(serv->ssdb, req, resp, 1);
}

static int proc_zdecr(Server *serv, Link *link, const Request &req, Response *resp){
	return _zincr(serv->ssdb, req, resp, -1);
}



