/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
/* queue */
#include "serv.h"
#include "net/proc.h"
#include "net/server.h"

int proc_qsize(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);

	int64_t ret = serv->ssdb->qsize(req[1]);
	resp->reply_int(ret, ret);
	return 0;
}

int proc_qfront(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);

	std::string item;
	int ret = serv->ssdb->qfront(req[1], &item);
	resp->reply_get(ret, &item);
	return 0;
}

int proc_qback(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);

	std::string item;
	int ret = serv->ssdb->qback(req[1], &item);
	resp->reply_get(ret, &item);
	return 0;
}

static int QFRONT = 2;
static int QBACK  = 3;

static inline
int proc_qpush_func(NetworkServer *net, Link *link, const Request &req, Response *resp, int front_or_back){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);

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
	}
	resp->reply_int(0, size);
	return 0;
}

int proc_qpush_front(NetworkServer *net, Link *link, const Request &req, Response *resp){
	return proc_qpush_func(net, link, req, resp, QFRONT);
}

int proc_qpush_back(NetworkServer *net, Link *link, const Request &req, Response *resp){
	return proc_qpush_func(net, link, req, resp, QBACK);
}

int proc_qpush(NetworkServer *net, Link *link, const Request &req, Response *resp){
	return proc_qpush_func(net, link, req, resp, QBACK);
}


static inline
int proc_qpop_func(NetworkServer *net, Link *link, const Request &req, Response *resp, int front_or_back){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);
	
	uint64_t size = 1;
	if(req.size() > 2){
		size = req[2].Uint64();
	}
		
	int ret;
	std::string item;

	if(size == 1){
		if(front_or_back == QFRONT){
			ret = serv->ssdb->qpop_front(req[1], &item);
		}else{
			ret = serv->ssdb->qpop_back(req[1], &item);
		}
		resp->reply_get(ret, &item);
	}else{
		resp->push_back("ok");
		while(size-- > 0){
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

int proc_qpop_front(NetworkServer *net, Link *link, const Request &req, Response *resp){
	return proc_qpop_func(net, link, req, resp, QFRONT);
}

int proc_qpop_back(NetworkServer *net, Link *link, const Request &req, Response *resp){
	return proc_qpop_func(net, link, req, resp, QBACK);
}

int proc_qpop(NetworkServer *net, Link *link, const Request &req, Response *resp){
	return proc_qpop_func(net, link, req, resp, QFRONT);
}

static inline
int proc_qtrim_func(NetworkServer *net, Link *link, const Request &req, Response *resp, int front_or_back){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);
	
	uint64_t size = 1;
	if(req.size() > 2){
		size = req[2].Uint64();
	}
		
	int count = 0;
	for(; count<size; count++){
		int ret;
		std::string item;
		if(front_or_back == QFRONT){
			ret = serv->ssdb->qpop_front(req[1], &item);
		}else{
			ret = serv->ssdb->qpop_back(req[1], &item);
		}
		if(ret <= 0){
			break;
		}
	}
	resp->reply_int(0, count);

	return 0;
}

int proc_qtrim_front(NetworkServer *net, Link *link, const Request &req, Response *resp){
	return proc_qtrim_func(net, link, req, resp, QFRONT);
}

int proc_qtrim_back(NetworkServer *net, Link *link, const Request &req, Response *resp){
	return proc_qtrim_func(net, link, req, resp, QBACK);
}

int proc_qlist(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	uint64_t limit = req[3].Uint64();
	std::vector<std::string> list;
	int ret = serv->ssdb->qlist(req[1], req[2], limit, &list);
	resp->reply_list(ret, list);
	return 0;
}

int proc_qrlist(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	uint64_t limit = req[3].Uint64();
	std::vector<std::string> list;
	int ret = serv->ssdb->qrlist(req[1], req[2], limit, &list);
	resp->reply_list(ret, list);
	return 0;
}

int proc_qfix(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);

	int ret = serv->ssdb->qfix(req[1]);
	if(ret == -1){
		resp->push_back("error");
	}else{
		resp->push_back("ok");
	}
	return 0;
}

int proc_qclear(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(2);

	int64_t count = 0;
	while(1){
		std::string item;
		int ret = serv->ssdb->qpop_front(req[1], &item);
		if(ret == 0){
			break;
		}
		if(ret == -1){
			return -1;
		}
		count += 1;
	}
	resp->reply_int(0, count);
	return 0;
}

int proc_qslice(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	int64_t begin = req[2].Int64();
	int64_t end = req[3].Int64();
	std::vector<std::string> list;
	int ret = serv->ssdb->qslice(req[1], begin, end, &list);
	resp->reply_list(ret, list);
	return 0;
}

int proc_qrange(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

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
	resp->reply_list(ret, list);
	return 0;
}

int proc_qget(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(3);

	int64_t index = req[2].Int64();
	std::string item;
	int ret = serv->ssdb->qget(req[1], index, &item);
	resp->reply_get(ret, &item);
	return 0;
}

int proc_qset(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	CHECK_NUM_PARAMS(4);

	const Bytes &name = req[1];
	int64_t index = req[2].Int64();
	const Bytes &item = req[3];
	int ret = serv->ssdb->qset(name, index, item);
	if(ret == -1 || ret == 0){
		resp->push_back("error");
		resp->push_back("index out of range");
	}else{
		resp->push_back("ok");
	}
	return 0;
}
