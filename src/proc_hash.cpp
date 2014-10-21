/* hash */
static int proc_hexists(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		const Bytes &name = req[1];
		const Bytes &key = req[2];
		std::string val;
		int ret = serv->ssdb->hget(name, key, &val);
		resp->reply_bool(ret);
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
				resp->add(ret);
			}
		}
	}
	return 0;
}

static int proc_multi_hset(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4 || req.size() % 2 != 0){
		resp->push_back("client_error");
	}else{
		int num = 0;
		const Bytes &name = req[1];
		std::vector<Bytes>::const_iterator it = req.begin() + 2;
		for(; it != req.end(); it += 2){
			const Bytes &key = *it;
			const Bytes &val = *(it + 1);
			int ret = serv->ssdb->hset(name, key, val);
			if(ret == -1){
				resp->push_back("error");
				return 0;
			}else{
				num += ret;
			}
		}
		resp->reply_int(0, num);
	}
	return 0;
}

static int proc_multi_hdel(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		int num = 0;
		const Bytes &name = req[1];
		std::vector<Bytes>::const_iterator it = req.begin() + 2;
		for(; it != req.end(); it += 1){
			const Bytes &key = *it;
			int ret = serv->ssdb->hdel(name, key);
			if(ret == -1){
				resp->push_back("error");
				return 0;
			}else{
				num += ret;
			}
		}
		resp->reply_int(0, num);
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
		resp->reply_int(ret, ret);
	}
	return 0;
}

static int proc_hset(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
	}else{
		int ret = serv->ssdb->hset(req[1], req[2], req[3]);
		resp->reply_bool(ret);
	}
	return 0;
}

static int proc_hget(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		std::string val;
		int ret = serv->ssdb->hget(req[1], req[2], &val);
		resp->reply_get(ret, &val);
	}
	return 0;
}

static int proc_hdel(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		int ret = serv->ssdb->hdel(req[1], req[2]);
		resp->reply_bool(ret);
	}
	return 0;
}

static int proc_hclear(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
		return 0;
	}
	
	const Bytes &name = req[1];
	int64_t count = 0;
	while(1){
		HIterator *it = serv->ssdb->hscan(name, "", "", 1000);
		int num = 0;
		while(it->next()){
			int ret = serv->ssdb->hdel(name, it->key);
			if(ret == -1){
				delete it;
				resp->push_back("error");
				return 0;
			}
			num ++;
		};
		delete it;

		if(num == 0){
			break;
		}
		count += num;
	}
	resp->reply_int(0, count);

	return 0;
}

static int proc_hgetall(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		HIterator *it = serv->ssdb->hscan(req[1], "", "", 2000000000);
		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
			resp->push_back(it->val);
		}
		delete it;
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

static int proc_hvals(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 5){
		resp->push_back("client_error");
	}else{
		uint64_t limit = req[4].Uint64();
		HIterator *it = serv->ssdb->hscan(req[1], req[2], req[3], limit);

		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->val);
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
		resp->reply_list(ret, list);
	}
	return 0;
}

static int proc_hrlist(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
	}else{
		uint64_t limit = req[3].Uint64();
		std::vector<std::string> list;
		int ret = serv->ssdb->hrlist(req[1], req[2], limit, &list);
		resp->reply_list(ret, list);
	}
	return 0;
}

// dir := +1|-1
static int _hincr(SSDB *ssdb, const Request &req, Response *resp, int dir){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		int64_t by = 1;
		if(req.size() > 3){
			by = req[3].Int64();
		}
		int64_t new_val;
		int ret = ssdb->hincr(req[1], req[2], dir * by, &new_val);
		resp->reply_int(ret, new_val);
	}
	return 0;
}

static int proc_hincr(Server *serv, Link *link, const Request &req, Response *resp){
	return _hincr(serv->ssdb, req, resp, 1);
}

static int proc_hdecr(Server *serv, Link *link, const Request &req, Response *resp){
	return _hincr(serv->ssdb, req, resp, -1);
}


