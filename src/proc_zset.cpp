/* zset */

static int proc_zexists(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		const Bytes &name = req[1];
		const Bytes &key = req[2];
		std::string val;
		int ret = serv->ssdb->zget(name, key, &val);
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
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		resp->push_back("ok");
		const Bytes &name = req[1];
		std::string val;
		for(Request::const_iterator it=req.begin()+2; it!=req.end(); it++){
			const Bytes &key = *it;
			int64_t ret = serv->ssdb->zget(name, key, &val);
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
				sprintf(buf, "%" PRId64 "", ret);
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
		int num = 0;
		const Bytes &name = req[1];
		std::vector<Bytes>::const_iterator it = req.begin() + 2;
		for(; it != req.end(); it += 2){
			const Bytes &key = *it;
			const Bytes &val = *(it + 1);
			int ret = serv->ssdb->zset(name, key, val);
			if(ret == -1){
				resp->push_back("error");
				return 0;
			}else{
				num += ret;
			}
		}
		resp->push_back("ok");
		char buf[20];
		sprintf(buf, "%d", num);
		resp->push_back(buf);
	}
	return 0;
}

static int proc_multi_zdel(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		int num = 0;
		const Bytes &name = req[1];
		std::vector<Bytes>::const_iterator it = req.begin() + 2;
		for(; it != req.end(); it += 1){
			const Bytes &key = *it;
			int ret = serv->ssdb->zdel(name, key);
			if(ret == -1){
				resp->push_back("error");
				return 0;
			}else{
				num += ret;
			}
		}
		resp->push_back("ok");
		char buf[20];
		sprintf(buf, "%d", num);
		resp->push_back(buf);
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
			sprintf(buf, "%" PRId64 "", ret);
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
			resp->push_back("error");
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

static int proc_zrank(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() != 3){
		resp->push_back("client_error");
	}else{
		int64_t ret = serv->ssdb->zrank(req[1], req[2]);
		char buf[20];
		sprintf(buf, "%" PRId64 "", ret);
		resp->push_back("ok");
		resp->push_back(buf);
	}
	return 0;
}

static int proc_zrrank(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() != 3){
		resp->push_back("client_error");
	}else{
		int64_t ret = serv->ssdb->zrrank(req[1], req[2]);
		char buf[20];
		sprintf(buf, "%" PRId64 "", ret);
		resp->push_back("ok");
		resp->push_back(buf);
	}
	return 0;
}

static int proc_zrange(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
	}else{
		uint64_t offset = req[2].Uint64();
		uint64_t limit = req[3].Uint64();
		ZIterator *it = serv->ssdb->zrange(req[1], offset, limit);
		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
			resp->push_back(it->score);
		}
		delete it;
	}
	return 0;
}

static int proc_zrrange(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
	}else{
		uint64_t offset = req[2].Uint64();
		uint64_t limit = req[3].Uint64();
		ZIterator *it = serv->ssdb->zrrange(req[1], offset, limit);
		resp->push_back("ok");
		while(it->next()){
			resp->push_back(it->key);
			resp->push_back(it->score);
		}
		delete it;
	}
	return 0;
}

static int proc_zclear(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 2){
		resp->push_back("client_error");
		return 0;
	}
	
	const Bytes &name = req[1];
	uint64_t total = 0;
	while(1){
		ZIterator *it = serv->ssdb->zrange(name, 0, 1000);
		int num = 0;
		while(it->next()){
			int ret = serv->ssdb->zdel(name, it->key);
			if(ret == -1){
				resp->push_back("error");
				delete it;
				return 0;
			}
			num ++;
		};
		delete it;
		
		if(num == 0){
			break;
		}
		total += num;
	}
	char buf[20];
	snprintf(buf, sizeof(buf), "%" PRIu64 "", total);
	resp->push_back("ok");
	resp->push_back(buf);

	return 0;
}

static int proc_zscan(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 6){
		resp->push_back("client_error");
	}else{
		uint64_t limit = req[5].Uint64();
		uint64_t offset = 0;
		if(req.size() > 6){
			offset = limit;
			limit = offset + req[6].Uint64();
		}
		ZIterator *it = serv->ssdb->zscan(req[1], req[2], req[3], req[4], limit);
		if(offset > 0){
			it->skip(offset);
		}
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
		uint64_t limit = req[5].Uint64();
		uint64_t offset = 0;
		if(req.size() > 6){
			offset = limit;
			limit = offset + req[6].Uint64();
		}
		ZIterator *it = serv->ssdb->zrscan(req[1], req[2], req[3], req[4], limit);
		if(offset > 0){
			it->skip(offset);
		}
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
		uint64_t limit = req[5].Uint64();
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
		uint64_t limit = req[3].Uint64();
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

static int proc_zcount(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
		return 0;
	}
	uint64_t count = 0;
	ZIterator *it = serv->ssdb->zscan(req[1], "", req[2], req[3], -1);
	while(it->next()){
		count ++;
	}
	delete it;
	
	char buf[20];
	snprintf(buf, sizeof(buf), "%" PRIu64 "", count);
	resp->push_back("ok");
	resp->push_back(buf);
	return 0;
}

static int proc_zsum(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
		return 0;
	}
	int64_t sum = 0;
	ZIterator *it = serv->ssdb->zscan(req[1], "", req[2], req[3], -1);
	while(it->next()){
		sum += str_to_int64(it->score);
	}
	delete it;
	
	char buf[20];
	snprintf(buf, sizeof(buf), "%" PRId64 "", sum);
	resp->push_back("ok");
	resp->push_back(buf);
	return 0;
}

static int proc_zavg(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
		return 0;
	}
	int64_t sum = 0;
	uint64_t count = 0;
	ZIterator *it = serv->ssdb->zscan(req[1], "", req[2], req[3], -1);
	while(it->next()){
		sum += str_to_int64(it->score);
		count ++;
	}
	delete it;
	
	double avg = (double)sum/count;
	char buf[20];
	snprintf(buf, sizeof(buf), "%f", avg);
	resp->push_back("ok");
	resp->push_back(buf);
	return 0;
}

static int proc_zremrangebyscore(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
		return 0;
	}
	ZIterator *it = serv->ssdb->zscan(req[1], "", req[2], req[3], -1);
	uint64_t count = 0;
	while(it->next()){
		count ++;
		int ret = serv->ssdb->zdel(req[1], it->key);
		if(ret == -1){
			resp->push_back("error");
			delete it;
			return 0;
		}
	}
	delete it;
	
	char buf[20];
	snprintf(buf, sizeof(buf), "%" PRIu64 "", count);
	resp->push_back("ok");
	resp->push_back(buf);
	return 0;
}

static int proc_zremrangebyrank(Server *serv, Link *link, const Request &req, Response *resp){
	if(req.size() < 4){
		resp->push_back("client_error");
		return 0;
	}
	uint64_t start = req[2].Uint64();
	uint64_t end = req[3].Uint64();
	ZIterator *it = serv->ssdb->zrange(req[1], start, end - start + 1);
	uint64_t count = 0;
	while(it->next()){
		count ++;
		int ret = serv->ssdb->zdel(req[1], it->key);
		if(ret == -1){
			resp->push_back("error");
			delete it;
			return 0;
		}
	}
	delete it;
	
	char buf[20];
	snprintf(buf, sizeof(buf), "%" PRIu64 "", count);
	resp->push_back("ok");
	resp->push_back(buf);
	return 0;
}

