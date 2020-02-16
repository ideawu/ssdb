/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef NET_LINK_ADDR_H_
#define NET_LINK_ADDR_H_

#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

struct LinkAddr
{
	bool ipv4;
	short family;
	socklen_t addrlen;
	
	LinkAddr(bool is_ipv4);
	LinkAddr(const char *ip, int port);
	void parse(const char *ip, int port);
	
	unsigned short port(){
		return ipv4? ntohs(addr4.sin_port) : ntohs(addr6.sin6_port);
	}
	struct sockaddr* addr(){
		return ipv4? (struct sockaddr *)&addr4 : (struct sockaddr *)&addr6;
	}
	void* sin_addr(){
		return ipv4? (void*)&addr4.sin_addr : (void*)&addr6.sin6_addr;
	}
	
private:
	struct sockaddr_in addr4;
	struct sockaddr_in6 addr6;

	LinkAddr(){};
	void is_ipv4(bool yn);
};

#endif
