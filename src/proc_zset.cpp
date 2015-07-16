/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
/* zset */
#include "serv.h"
#include "net/proc.h"
#include "net/server.h"

int proc_zexists(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);

	const Bytes &name = req[1];
	const Bytes &key = req[2];
	std::string val;
	int ret = serv->ssdb->zget(name, key, &val);
	resp->reply_bool(ret);
	return 0;
}

int proc_multi_zexists(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);

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
	return 0;
}

int proc_multi_zsize(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);

	resp->push_back("ok");
	for(Request::const_iterator it=req.begin()+1; it!=req.end(); it++){
		const Bytes &key = *it;
		int64_t ret = serv->ssdb->zsize(key);
		resp->push_back(key.String());
		if(ret == -1){
			resp->push_back("-1");
		}else{
			resp->add(ret);
		}
	}
	return 0;
}

int proc_multi_zset(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
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
		resp->reply_int(0, num);
	}
	return 0;
}

int proc_multi_zdel(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);

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
	resp->reply_int(0, num);
	return 0;
}

int proc_multi_zget(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);

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
		}
	}
	return 0;
}

int proc_zset(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	int ret = serv->ssdb->zset(req[1], req[2], req[3]);
	resp->reply_int(ret, ret);
	return 0;
}

int proc_zsize(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);

	int64_t ret = serv->ssdb->zsize(req[1]);
	resp->reply_int(ret, ret);
	return 0;
}

int proc_zget(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);

	std::string score;
	int ret = serv->ssdb->zget(req[1], req[2], &score);
	resp->reply_get(ret, &score);
	return 0;
}

int proc_zdel(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);

	int ret = serv->ssdb->zdel(req[1], req[2]);
	resp->reply_bool(ret);
	return 0;
}

int proc_zrank(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);

	int64_t ret = serv->ssdb->zrank(req[1], req[2]);
	if(ret == -1){
		resp->add("not_found");
	}else{
		resp->reply_int(ret, ret);
	}
	return 0;
}

int proc_zrrank(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);

	int64_t ret = serv->ssdb->zrrank(req[1], req[2]);
	if(ret == -1){
		resp->add("not_found");
	}else{
		resp->reply_int(ret, ret);
	}
	return 0;
}

int proc_zrange(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	uint64_t offset = req[2].Uint64();
	uint64_t limit = req[3].Uint64();
	ZIterator *it = serv->ssdb->zrange(req[1], offset, limit);
	resp->push_back("ok");
	while(it->next()){
		resp->push_back(it->key);
		resp->push_back(it->score);
	}
	delete it;
	return 0;
}

int proc_zrrange(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	uint64_t offset = req[2].Uint64();
	uint64_t limit = req[3].Uint64();
	ZIterator *it = serv->ssdb->zrrange(req[1], offset, limit);
	resp->push_back("ok");
	while(it->next()){
		resp->push_back(it->key);
		resp->push_back(it->score);
	}
	delete it;
	return 0;
}

int proc_zclear(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);
	
	const Bytes &name = req[1];
	int64_t count = 0;
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
		count += num;
	}
	resp->reply_int(0, count);

	return 0;
}

int proc_zscan(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(6);

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
	return 0;
}

int proc_zrscan(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(6);

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
	return 0;
}

int proc_zkeys(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(6);

	uint64_t limit = req[5].Uint64();
	ZIterator *it = serv->ssdb->zscan(req[1], req[2], req[3], req[4], limit);
	resp->push_back("ok");
	while(it->next()){
		resp->push_back(it->key);
	}
	delete it;
	return 0;
}

int proc_zlist(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	uint64_t limit = req[3].Uint64();
	std::vector<std::string> list;
	int ret = serv->ssdb->zlist(req[1], req[2], limit, &list);
	resp->reply_list(ret, list);
	return 0;
}

int proc_zrlist(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	uint64_t limit = req[3].Uint64();
	std::vector<std::string> list;
	int ret = serv->ssdb->zrlist(req[1], req[2], limit, &list);
	resp->reply_list(ret, list);
	return 0;
}

// dir := +1|-1
static int _zincr(SSDB *ssdb, const Request &req, Response *resp, int dir){
	CHECK_NUM_PARAMS(3);

	int64_t by = 1;
	if(req.size() > 3){
		by = req[3].Int64();
	}
	int64_t new_val;
	int ret = ssdb->zincr(req[1], req[2], dir * by, &new_val);
	resp->reply_int(ret, new_val);
	return 0;
}

int proc_zincr(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	return _zincr(serv->ssdb, req, resp, 1);
}

int proc_zdecr(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	return _zincr(serv->ssdb, req, resp, -1);
}

int proc_zcount(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	int64_t count = 0;
	ZIterator *it = serv->ssdb->zscan(req[1], "", req[2], req[3], -1);
	while(it->next()){
		count ++;
	}
	delete it;
	
	resp->reply_int(0, count);
	return 0;
}

int proc_zsum(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	int64_t sum = 0;
	ZIterator *it = serv->ssdb->zscan(req[1], "", req[2], req[3], -1);
	while(it->next()){
		sum += str_to_int64(it->score);
	}
	delete it;
	
	resp->reply_int(0, sum);
	return 0;
}

int proc_zavg(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	int64_t sum = 0;
	int64_t count = 0;
	ZIterator *it = serv->ssdb->zscan(req[1], "", req[2], req[3], -1);
	while(it->next()){
		sum += str_to_int64(it->score);
		count ++;
	}
	delete it;
	double avg = (double)sum/count;
	
	resp->push_back("ok");
	resp->add(avg);
	return 0;
}

int proc_zremrangebyscore(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	ZIterator *it = serv->ssdb->zscan(req[1], "", req[2], req[3], -1);
	int64_t count = 0;
	while(it->next()){
		count ++;
		int ret = serv->ssdb->zdel(req[1], it->key);
		if(ret == -1){
			delete it;
			resp->push_back("error");
			return 0;
		}
	}
	delete it;
	
	resp->reply_int(0, count);
	return 0;
}

int proc_zremrangebyrank(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	uint64_t start = req[2].Uint64();
	uint64_t end = req[3].Uint64();
	ZIterator *it = serv->ssdb->zrange(req[1], start, end - start + 1);
	int64_t count = 0;
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
	
	resp->reply_int(0, count);
	return 0;
}

// 因为 writer 线程只有一个, 所以这个操作还是线程安全的
static inline
void zpop(ZIterator *it, SSDBServer *serv, const Bytes &name, Response *resp){
	resp->push_back("ok");
	while(it->next()){
		int ret = serv->ssdb->zdel(name, it->key);
		if(ret == 1){
			resp->add(it->key);
			resp->add(it->score);
		}
	}
}

int proc_zpop_front(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);

	const Bytes &name = req[1];
	uint64_t limit = req[2].Uint64();
	ZIterator *it = serv->ssdb->zscan(name, "", "", "", limit);
	zpop(it, serv, name, resp);
	delete it;
	return 0;
}

int proc_zpop_back(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);

	const Bytes &name = req[1];
	uint64_t limit = req[2].Uint64();
	ZIterator *it = serv->ssdb->zrscan(name, "", "", "", limit);
	zpop(it, serv, name, resp);
	delete it;
	return 0;
}

