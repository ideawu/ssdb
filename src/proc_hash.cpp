/* hash */
static int proc_hexists(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		const Bytes &name = req[1];
		const Bytes &key = req[2];
		std::string val;
		int ret = serv->ssdb->hget(name, key, &val);
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

static int proc_multi_hexists(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		resp->push_back("ok");
		const Bytes &name = req[1];
		std::string val;
		for(Request::const_iterator it=req.begin()+2; it!=req.end(); it++){
			const Bytes &key = *it;
			int64_t ret = serv->ssdb->hget(name, key, &val);
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

static int proc_multi_hsize(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		resp->push_back("ok");
		for(Request::const_iterator it=req.begin()+1; it!=req.end(); it++){
			const Bytes &key = *it;
			int64_t ret = serv->ssdb->hsize(key);
			resp->push_back(key.String());
			if(ret == -1){
				resp->push_back("-1");
			}else{
				char buf[20];
				sprintf(buf, "%"PRId64"", ret);
				resp->push_back(buf);
			}
		}
	}
	return 0;
}

static int proc_multi_hset(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4 || req.size() % 2 != 0){
		resp->push_back("client_error");
	}else{
		const Bytes &name = req[1];
		int ret = serv->ssdb->multi_hset(name, req, 2);
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

static int proc_multi_hdel(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		const Bytes &name = req[1];
		int ret = serv->ssdb->multi_hdel(name, req, 2);
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

static int proc_multi_hget(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		resp->push_back("ok");
		Request::const_iterator it=req.begin() + 1;
		const Bytes name = *it;
		it ++;
		for(; it!=req.end(); it+=1){
			const Bytes &key = *it;
			std::string val;
			int ret = serv->ssdb->hget(name, key, &val);
			if(ret == 1){
				resp->push_back(key.String());
				resp->push_back(val);
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

static int proc_hsize(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		int64_t ret = serv->ssdb->hsize(req[1]);
		if(ret == -1){
			resp->push_back("error");
		}else{
			char buf[20];
			sprintf(buf, "%"PRIu64"", ret);
			resp->push_back("ok");
			resp->push_back(buf);
		}
	}
	return 0;
}

static int proc_hset(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
	}else{
		int ret = serv->ssdb->hset(req[1], req[2], req[3]);
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

static int proc_hget(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		std::string val;
		int ret = serv->ssdb->hget(req[1], req[2], &val);
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

static int proc_hdel(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		int ret = serv->ssdb->hdel(req[1], req[2]);
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

static int proc_hscan(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 5){
		resp->push_back("client_error");
	}else{
		uint64_t limit = req[4].Uint64();
		HIterator *it = serv->ssdb->hscan(req[1], req[2], req[3], limit);
		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
			resp->push_back(it->val);
		}
		delete it;
	}
	return 0;
}

static int proc_hrscan(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 5){
		resp->push_back("client_error");
	}else{
		uint64_t limit = req[4].Uint64();
		HIterator *it = serv->ssdb->hrscan(req[1], req[2], req[3], limit);
		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
			resp->push_back(it->val);
		}
		delete it;
	}
	return 0;
}

static int proc_hkeys(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 5){
		resp->push_back("client_error");
	}else{
		uint64_t limit = req[4].Uint64();
		HIterator *it = serv->ssdb->hscan(req[1], req[2], req[3], limit);
		it->return_val(false);

		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
		}
		delete it;
	}
	return 0;
}

static int proc_hlist(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
	}else{
		uint64_t limit = req[3].Uint64();
		std::vector<std::string> list;
		int ret = serv->ssdb->hlist(req[1], req[2], limit, &list);
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
static int _hincr(SSDB *ssdb, const Request &req, Response *resp, int dir){
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

static int proc_hincr(Server *serv, Link *link, const Request &req, Response *resp){
	return _hincr(serv->ssdb, req, resp, 1);
}

static int proc_hdecr(Server *serv, Link *link, const Request &req, Response *resp){
	return _hincr(serv->ssdb, req, resp, -1);
}


