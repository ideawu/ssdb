/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
/* nset */
#include "serv.h"
#include "net/proc.h"
#include "net/server.h"

int proc_nexists(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);

	const Bytes &name = req[1];
	const Bytes &score = req[2];
	std::string val;
	int ret = serv->ssdb->nget(name, score, &val);
	resp->reply_bool(ret);
	return 0;
}

int proc_nmset(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	if(req.size() < 4 || req.size() % 2 != 0){
		resp->push_back("client_error");
	}else{
		int num = 0;
		const Bytes &name = req[1];
		std::vector<Bytes>::const_iterator it = req.begin() + 2;
		for(; it != req.end(); it += 2){
			const Bytes &val = *it;
			const Bytes &score = *(it + 1);
			int ret = serv->ssdb->nset(name, val, score);
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

#define encode_score(s) big_endian((uint64_t)(s))
int proc_nset(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;

	if(req.size() != 4 && req.size() != 5 ){
		resp->push_back("client_error");
		resp->push_back("wrong number of arguments");
		return 0;
	}

	int ret = serv->ssdb->nset(req[1], req[2], req[3]);

	if (req.size() == 5){
		Locking l(&serv->expiration->mutex);

		std::string buf;
		buf.append(1, DataType::NSET);
		buf.append(1, (uint8_t)req[1].size());
		buf.append(req[1].data(), req[1].size());

		int64_t s = req[3].Int64();
		if(s < 0){
			buf.append(1, '-');
		}else{
			buf.append(1, '=');
		}
		s = encode_score(s);

		buf.append((char *)&s, sizeof(int64_t));


		std::string val;
		int ret = serv->ssdb->nget(req[1], req[3], &val);
		if (ret == 1) {
			serv->expiration->set_ttl(Bytes(buf), req[4].Int());
		}
	}
	resp->reply_int(ret, ret);
	return 0;
}

int proc_nsize(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);

	int64_t ret = serv->ssdb->nsize(req[1]);
	resp->reply_int(ret, ret);
	return 0;
}

int proc_nget(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);

	std::string val;
	int ret = serv->ssdb->nget(req[1], req[2], &val);
	resp->reply_get(ret, &val);
	return 0;
}

int proc_nrange(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	uint64_t offset = req[2].Uint64();
	uint64_t limit = req[3].Uint64();
	NIterator *it = serv->ssdb->nrange(req[1], offset, limit);
	resp->push_back("ok");
	while(it->next()){
		resp->push_back(it->val);
		resp->push_back(it->score);
	}
	delete it;
	return 0;
}

int proc_nrrange(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	uint64_t offset = req[2].Uint64();
	uint64_t limit = req[3].Uint64();
	NIterator *it = serv->ssdb->nrrange(req[1], offset, limit);
	resp->push_back("ok");
	while(it->next()){
		resp->push_back(it->val);
		resp->push_back(it->score);
	}
	delete it;
	return 0;
}

int proc_ndel(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);

	Locking l(&serv->expiration->mutex);
	int ret = serv->ssdb->ndel(req[1], req[2]);
	if(ret == -1){
		resp->push_back("error");
	}else{
		std::string buf;
		buf.append(1, DataType::NSET);
		buf.append(1, (uint8_t)req[1].size());
		buf.append(req[1].data(), req[1].size());

		int64_t s = req[2].Int64();
		if(s < 0){
			buf.append(1, '-');
		}else{
			buf.append(1, '=');
		}
		s = encode_score(s);

		buf.append((char *)&s, sizeof(int64_t));

		serv->expiration->del_ttl(Bytes(buf));

		resp->push_back("ok");
		resp->push_back("1");
	}
	return 0;
}

int proc_nmdel(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	if(req.size() < 3){
		resp->push_back("client_error");
	}else{
		int num = 0;
		const Bytes &name = req[1];
		std::vector<Bytes>::const_iterator it = req.begin() + 2;
		for(; it != req.end(); it += 1){
			const Bytes &score = *it;
			int ret = serv->ssdb->ndel(name, score);
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

int proc_nclear(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);
	
	const Bytes &name = req[1];
	int64_t count = 0;
	while(1){
		NIterator *it = serv->ssdb->nrange(name, 0, 1000);
		int num = 0;
		while(it->next()){
			int ret = serv->ssdb->ndel(name, it->score);
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

int proc_nscan(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(5);

	uint64_t limit = req[4].Uint64();
	uint64_t offset = 0;
	if(req.size() > 5){
		offset = limit;
		limit = offset + req[5].Uint64();
	}
	NIterator *it = serv->ssdb->nscan(req[1], req[2], req[3], limit);
	if(offset > 0){
		it->skip(offset);
	}
	resp->push_back("ok");
	while(it->next()){
		resp->push_back(it->val);
		resp->push_back(it->score);
	}
	delete it;
	return 0;
}

int proc_nrscan(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(5);

	uint64_t limit = req[4].Uint64();
	uint64_t offset = 0;
	if(req.size() > 6){
		offset = limit;
		limit = offset + req[5].Uint64();
	}
	NIterator *it = serv->ssdb->nrscan(req[1], req[2], req[3], limit);
	if(offset > 0){
		it->skip(offset);
	}
	resp->push_back("ok");
	while(it->next()){
		resp->push_back(it->val);
		resp->push_back(it->score);
	}
	delete it;
	return 0;
}

int proc_nlist(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	uint64_t limit = req[3].Uint64();
	std::vector<std::string> list;
	int ret = serv->ssdb->nlist(req[1], req[2], limit, &list);
	resp->reply_list(ret, list);
	return 0;
}

int proc_nrlist(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	uint64_t limit = req[3].Uint64();
	std::vector<std::string> list;
	int ret = serv->ssdb->nrlist(req[1], req[2], limit, &list);
	resp->reply_list(ret, list);
	return 0;
}


int proc_nsum(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	int64_t sum = 0;
	NIterator *it = serv->ssdb->nscan(req[1], req[2], req[3], -1);
	while(it->next()){
		sum += str_to_int64(it->score);
	}
	delete it;
	
	resp->reply_int(0, sum);
	return 0;
}

int proc_navg(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	int64_t sum = 0;
	int64_t count = 0;
	NIterator *it = serv->ssdb->nscan(req[1], req[2], req[3], -1);
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

int proc_nremrangebyscore(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	NIterator *it = serv->ssdb->nscan(req[1], req[2], req[3], -1);
	int64_t count = 0;
	while(it->next()){
		count ++;
		int ret = serv->ssdb->ndel(req[1], it->score);
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

int proc_nremrangebyrank(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	uint64_t start = req[2].Uint64();
	uint64_t end = req[3].Uint64();
	NIterator *it = serv->ssdb->nrange(req[1], start, end - start + 1);
	int64_t count = 0;
	while(it->next()){
		count ++;
		int ret = serv->ssdb->ndel(req[1], it->score);
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

// ���涓� writer 绾跨��������涓�涓�, ���浠ヨ��涓����浣�杩����绾跨��瀹���ㄧ��
static inline
void npop(NIterator *it, SSDBServer *serv, const Bytes &name, Response *resp){
	resp->push_back("ok");
	while(it->next()){
		int ret = serv->ssdb->ndel(name, it->score);
		if(ret == 1){
			resp->add(it->val);
			resp->add(it->score);
		}
	}
}

int proc_npop_front(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);

	const Bytes &name = req[1];
	uint64_t limit = req[2].Uint64();
	NIterator *it = serv->ssdb->nscan(name, "", "", limit);
	npop(it, serv, name, resp);
	delete it;
	return 0;
}

int proc_npop_back(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);

	const Bytes &name = req[1];
	uint64_t limit = req[2].Uint64();
	NIterator *it = serv->ssdb->nrscan(name, "", "", limit);
	npop(it, serv, name, resp);
	delete it;
	return 0;
}

