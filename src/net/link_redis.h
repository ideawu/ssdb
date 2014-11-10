/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef NET_REDIS_LINK_H_
#define NET_REDIS_LINK_H_

#include <vector>
#include <string>
#include "../util/bytes.h"

struct RedisRequestDesc
{
	int strategy;
	std::string redis_cmd;
	std::string ssdb_cmd;
	int reply_type;
};

class RedisLink
{
private:
	std::string cmd;
	RedisRequestDesc *req_desc;

	std::vector<Bytes> recv_bytes;
	std::vector<std::string> recv_string;
	int parse_req(Buffer *input);
	int convert_req();
	
public:
	RedisLink(){
		req_desc = NULL;
	}
	
	const std::vector<Bytes>* recv_req(Buffer *input);
	int send_resp(Buffer *output, const std::vector<std::string> &resp);
};

#endif
