/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
/* kv */
#include "serv.h"
#include "net/proc.h"
#include "net/server.h"

int proc_get(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);
	CHECK_KV_KEY_RANGE(1);

	std::string val;
	int ret = serv->ssdb->get(req[1], &val);
	resp->reply_get(ret, &val);
	return 0;
}

int proc_getset(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);
	CHECK_KV_KEY_RANGE(1);

	std::string val;
	int ret = serv->ssdb->getset(req[1], &val, req[2]);
	resp->reply_get(ret, &val);
	return 0;
}

int proc_set(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);
	CHECK_KV_KEY_RANGE(1);

	int ret = serv->ssdb->set(req[1], req[2]);
	if(ret == -1){
		resp->push_back("error");
	}else{
		resp->push_back("ok");
		resp->push_back("1");
	}
	return 0;
}

int proc_setnx(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);
	CHECK_KV_KEY_RANGE(1);

	int ret = serv->ssdb->setnx(req[1], req[2]);
	resp->reply_bool(ret);
	return 0;
}

int proc_setx(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);
	CHECK_KV_KEY_RANGE(1);

	Locking l(&serv->expiration->mutex);
	int ret;
	ret = serv->ssdb->set(req[1], req[2]);
	if(ret == -1){
		resp->push_back("error");
		return 0;
	}
	ret = serv->expiration->set_ttl(req[1], req[3].Int());
	if(ret == -1){
		resp->push_back("error");
	}else{
		resp->push_back("ok");
		resp->push_back("1");
	}
	return 0;
}

int proc_ttl(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);
	CHECK_KV_KEY_RANGE(1);

	int64_t ttl = serv->expiration->get_ttl(req[1]);
	resp->push_back("ok");
	resp->push_back(str(ttl));
	return 0;
}

int proc_expire(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);
	CHECK_KV_KEY_RANGE(1);

	Locking l(&serv->expiration->mutex);
	std::string val;
	int ret = serv->ssdb->get(req[1], &val);
	if(ret == 1){
		ret = serv->expiration->set_ttl(req[1], req[2].Int());
		if(ret != -1){
			resp->push_back("ok");
			resp->push_back("1");
			return 0;
		}
	}
	resp->push_back("ok");
	resp->push_back("0");
	return 0;
}

int proc_exists(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);
	CHECK_KV_KEY_RANGE(1);

	const Bytes key = req[1];
	std::string val;
	int ret = serv->ssdb->get(key, &val);
	resp->reply_bool(ret);
	return 0;
}

int proc_multi_exists(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);

	resp->push_back("ok");
	for(Request::const_iterator it=req.begin()+1; it!=req.end(); it++){
		const Bytes key = *it;
		std::string val;
		int ret = serv->ssdb->get(key, &val);
		resp->push_back(key.String());
		if(ret == 1){
			resp->push_back("1");
		}else if(ret == 0){
			resp->push_back("0");
		}else{
			resp->push_back("0");
		}
	}
	return 0;
}

int proc_multi_set(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	if(req.size() < 3 || req.size() % 2 != 1){
		resp->push_back("client_error");
	}else{
		int ret = serv->ssdb->multi_set(req, 1);
		resp->reply_int(0, ret);
	}
	return 0;
}

int proc_multi_del(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);

	Locking l(&serv->expiration->mutex);
	int ret = serv->ssdb->multi_del(req, 1);
	if(ret == -1){
		resp->push_back("error");
	}else{
		for(Request::const_iterator it=req.begin()+1; it!=req.end(); it++){
			const Bytes key = *it;
			serv->expiration->del_ttl(key);
		}
		resp->reply_int(0, ret);
	}
	return 0;
}

int proc_multi_get(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);

	resp->push_back("ok");
	for(int i=1; i<req.size(); i++){
		std::string val;
		int ret = serv->ssdb->get(req[i], &val);
		if(ret == 1){
			resp->push_back(req[i].String());
			resp->push_back(val);
		}
	}
	return 0;
}

int proc_del(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);
	CHECK_KV_KEY_RANGE(1);

	Locking l(&serv->expiration->mutex);
	int ret = serv->ssdb->del(req[1]);
	if(ret == -1){
		resp->push_back("error");
	}else{
		serv->expiration->del_ttl(req[1]);
			
		resp->push_back("ok");
		resp->push_back("1");
	}
	return 0;
}

int proc_scan(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	uint64_t limit = req[3].Uint64();
	KIterator *it = serv->ssdb->scan(req[1], req[2], limit);
	resp->push_back("ok");
	while(it->next()){
		resp->push_back(it->key);
		resp->push_back(it->val);
	}
	delete it;
	return 0;
}

int proc_rscan(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	uint64_t limit = req[3].Uint64();
	KIterator *it = serv->ssdb->rscan(req[1], req[2], limit);
	resp->push_back("ok");
	while(it->next()){
		resp->push_back(it->key);
		resp->push_back(it->val);
	}
	delete it;
	return 0;
}

int proc_keys(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	uint64_t limit = req[3].Uint64();
	KIterator *it = serv->ssdb->scan(req[1], req[2], limit);
	it->return_val(false);

	resp->push_back("ok");
	while(it->next()){
		resp->push_back(it->key);
	}
	delete it;
	return 0;
}

int proc_rkeys(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	uint64_t limit = req[3].Uint64();
	KIterator *it = serv->ssdb->rscan(req[1], req[2], limit);
	it->return_val(false);

	resp->push_back("ok");
	while(it->next()){
		resp->push_back(it->key);
	}
	delete it;
	return 0;
}

// dir := +1|-1
static int _incr(SSDB *ssdb, const Request &req, Response *resp, int dir){
	CHECK_NUM_PARAMS(2);
	int64_t by = 1;
	if(req.size() > 2){
		by = req[2].Int64();
	}
	int64_t new_val;
	int ret = ssdb->incr(req[1], dir * by, &new_val);
	if(ret == 0){
		resp->reply_status(-1, "value is not an integer or out of range");
	}else{
		resp->reply_int(ret, new_val);
	}
	return 0;
}

int proc_incr(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_KV_KEY_RANGE(1);
	return _incr(serv->ssdb, req, resp, 1);
}

int proc_decr(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_KV_KEY_RANGE(1);
	return _incr(serv->ssdb, req, resp, -1);
}

int proc_getbit(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);
	CHECK_KV_KEY_RANGE(1);

	int ret = serv->ssdb->getbit(req[1], req[2].Int());
	resp->reply_bool(ret);
	return 0;
}

int proc_setbit(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);
	CHECK_KV_KEY_RANGE(1);

	const Bytes &key = req[1];
	int offset = req[2].Int();
	if(req[3].size() == 0 || (req[3].data()[0] != '0' && req[3].data()[0] != '1')){
		resp->push_back("client_error");
		resp->push_back("bit is not an integer or out of range");
		return 0;
	}
	int on = req[3].Int();
	int ret = serv->ssdb->setbit(key, offset, on);
	resp->reply_bool(ret);
	return 0;
}

int proc_countbit(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);
	CHECK_KV_KEY_RANGE(1);

	const Bytes &key = req[1];
	int start = 0;
	if(req.size() > 2){
		start = req[2].Int();
	}
	std::string val;
	int ret = serv->ssdb->get(key, &val);
	if(ret == -1){
		resp->push_back("error");
	}else{
		std::string str;
		int size = -1;
		if(req.size() > 3){
			size = req[3].Int();
			str = substr(val, start, size);
		}else{
			str = substr(val, start, val.size());
		}
		int count = bitcount(str.data(), str.size());
		resp->reply_int(0, count);
	}
	return 0;
}

int proc_bitcount(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);
	CHECK_KV_KEY_RANGE(1);

	const Bytes &key = req[1];
	int start = 0;
	if(req.size() > 2){
		start = req[2].Int();
	}
	int end = -1;
	if(req.size() > 3){
		end = req[3].Int();
	}
	std::string val;
	int ret = serv->ssdb->get(key, &val);
	if(ret == -1){
		resp->push_back("error");
	}else{
		std::string str = str_slice(val, start, end);
		int count = bitcount(str.data(), str.size());
		resp->reply_int(0, count);
	}
	return 0;
}

int proc_substr(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);
	CHECK_KV_KEY_RANGE(1);

	const Bytes &key = req[1];
	int start = 0;
	if(req.size() > 2){
		start = req[2].Int();
	}
	int size = 2000000000;
	if(req.size() > 3){
		size = req[3].Int();
	}
	std::string val;
	int ret = serv->ssdb->get(key, &val);
	if(ret == -1){
		resp->push_back("error");
	}else{
		std::string str = substr(val, start, size);
		resp->push_back("ok");
		resp->push_back(str);
	}
	return 0;
}

int proc_getrange(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);
	CHECK_KV_KEY_RANGE(1);

	const Bytes &key = req[1];
	int start = 0;
	if(req.size() > 2){
		start = req[2].Int();
	}
	int size = -1;
	if(req.size() > 3){
		size = req[3].Int();
	}
	std::string val;
	int ret = serv->ssdb->get(key, &val);
	if(ret == -1){
		resp->push_back("error");
	}else{
		std::string str = str_slice(val, start, size);
		resp->push_back("ok");
		resp->push_back(str);
	}
	return 0;
}

int proc_strlen(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);
	CHECK_KV_KEY_RANGE(1);

	const Bytes &key = req[1];
	std::string val;
	int ret = serv->ssdb->get(key, &val);
	resp->reply_int(ret, val.size());
	return 0;
}
