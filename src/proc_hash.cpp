/* hash */
int CommandProc::proc_hexists(const Link &link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		int64_t ret = ssdb->hsize(req[1]);
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

int CommandProc::proc_multi_hexists(const Link &link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		resp->push_back("ok");
		for(Request::const_iterator it=req.begin()+1; it!=req.end(); it++){
			const Bytes &key = *it;
			int64_t ret = ssdb->hsize(key);
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

int CommandProc::proc_multi_hsize(const Link &link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		resp->push_back("ok");
		for(Request::const_iterator it=req.begin()+1; it!=req.end(); it++){
			const Bytes &key = *it;
			int64_t ret = ssdb->hsize(key);
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

int CommandProc::proc_multi_hset(const Link &link, const Request &req, Response *resp){
	if(req.size() < 4 || req.size() % 2 != 0){
		resp->push_back("client_error");
	}else{
		const Bytes &name = req[1];
		int ret = ssdb->multi_hset(name, req, 2);
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

int CommandProc::proc_multi_hdel(const Link &link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		const Bytes &name = req[1];
		int ret = ssdb->multi_hdel(name, req, 2);
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

int CommandProc::proc_multi_hget(const Link &link, const Request &req, Response *resp){
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
			int ret = ssdb->hget(name, key, &val);
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

int CommandProc::proc_hsize(const Link &link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
	}else{
		int64_t ret = ssdb->hsize(req[1]);
		if(ret == -1){
			resp->push_back("error");
		}else{
			char buf[20];
			sprintf(buf, "%llu", ret);
			resp->push_back("ok");
			resp->push_back(buf);
		}
	}
	return 0;
}

int CommandProc::proc_hset(const Link &link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		int ret = ssdb->hset(req[1], req[2], req[3]);
		if(ret == -1){
			resp->push_back("error");
		}else{
			resp->push_back("ok");
		}
	}
	return 0;
}

int CommandProc::proc_hget(const Link &link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		std::string val;
		int ret = ssdb->hget(req[1], req[2], &val);
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

int CommandProc::proc_hdel(const Link &link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		int ret = ssdb->hdel(req[1], req[2]);
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

int CommandProc::proc_hscan(const Link &link, const Request &req, Response *resp){
	if(req.size() < 5){
		resp->push_back("client_error");
	}else{
		int limit = req[4].Int();
		HIterator *it = ssdb->hscan(req[1], req[2], req[3], limit);
		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
			resp->push_back(it->val);
		}
		delete it;
	}
	return 0;
}

int CommandProc::proc_hrscan(const Link &link, const Request &req, Response *resp){
	if(req.size() < 5){
		resp->push_back("client_error");
	}else{
		int limit = req[4].Int();
		HIterator *it = ssdb->hrscan(req[1], req[2], req[3], limit);
		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
			resp->push_back(it->val);
		}
		delete it;
	}
	return 0;
}

int CommandProc::proc_hkeys(const Link &link, const Request &req, Response *resp){
	if(req.size() < 5){
		resp->push_back("client_error");
	}else{
		int limit = req[4].Int();
		HIterator *it = ssdb->hscan(req[1], req[2], req[3], limit);
		it->return_val(false);

		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
		}
		delete it;
	}
	return 0;
}

int CommandProc::proc_hlist(const Link &link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
	}else{
		int limit = req[3].Int();
		std::vector<std::string> list;
		int ret = ssdb->hlist(req[1], req[2], limit, &list);
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

int CommandProc::proc_hincr(const Link &link, const Request &req, Response *resp){
	return _hincr(ssdb, req, resp, 1);
}

int CommandProc::proc_hdecr(const Link &link, const Request &req, Response *resp){
	return _hincr(ssdb, req, resp, -1);
}


