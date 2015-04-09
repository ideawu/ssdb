/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "resp.h"
#include <stdio.h>

int Response::size() const{
	return (int)resp.size();
}

void Response::push_back(const std::string &s){
	resp.push_back(s);
}

void Response::add(int s){
	add((int64_t)s);
}

void Response::add(int64_t s){
	char buf[20];
	sprintf(buf, "%" PRId64 "", s);
	resp.push_back(buf);
}

void Response::add(uint64_t s){
	char buf[20];
	sprintf(buf, "%" PRIu64 "", s);
	resp.push_back(buf);
}

void Response::add(double s){
	char buf[30];
	snprintf(buf, sizeof(buf), "%f", s);
	resp.push_back(buf);
}

void Response::add(const std::string &s){
	resp.push_back(s);
}

void Response::reply_status(int status, const char *errmsg){
	if(status == -1){
		resp.push_back("error");
		if(errmsg){
			resp.push_back(errmsg);
		}
	}else{
		resp.push_back("ok");
	}
}

void Response::reply_bool(int status, const char *errmsg){
	if(status == -1){
		resp.push_back("error");
		if(errmsg){
			resp.push_back(errmsg);
		}
	}else if(status == 0){
		resp.push_back("ok");
		resp.push_back("0");
	}else{
		resp.push_back("ok");
		resp.push_back("1");
	}
}

void Response::reply_int(int status, int64_t val){
	if(status == -1){
		resp.push_back("error");
	}else{
		resp.push_back("ok");
		this->add(val);
	}
}

void Response::reply_get(int status, const std::string *val, const char *errmsg){
	if(status == -1){
		resp.push_back("error");
	}else if(status == 0){
		resp.push_back("not_found");
	}else{
		resp.push_back("ok");
		if(val){
			resp.push_back(*val);
		}
		return;
	}
	if(errmsg){
		resp.push_back(errmsg);
	}
} 

void Response::reply_list(int status, const std::vector<std::string> &list){
	if(status == -1){
		resp.push_back("error");
	}else{
		resp.push_back("ok");
		for(int i=0; i<list.size(); i++){
			resp.push_back(list[i]);
		}
	}
}

