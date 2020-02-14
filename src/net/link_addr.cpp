#include <string.h>
#include <stdio.h>
#include "link_addr.h"

LinkAddr::LinkAddr(const char *ip, int port){
	parse(ip, port);
}

LinkAddr::LinkAddr(bool is_ipv4){
	this->is_ipv4(is_ipv4);
}

void LinkAddr::is_ipv4(bool yn){
	if(yn){
		ipv4 = true;
		family = AF_INET;
		addrlen = sizeof(addr4);
	}else{
		ipv4 = false;
		family = AF_INET6;
		addrlen = sizeof(addr6);
	}
}

void LinkAddr::parse(const char *ip, int port){
	this->is_ipv4(strchr(ip, ':') == NULL);
	if(ipv4){
		bzero(&addr4, sizeof(addr4));
		addr4.sin_family = family;
		addr4.sin_port = htons((short)port);
		inet_pton(family, ip, &addr4.sin_addr);
	}else{
		bzero(&addr6, sizeof(addr6));
		addr6.sin6_family = family;
		addr6.sin6_port = htons((short)port);
		inet_pton(family, ip, &addr6.sin6_addr);
	}
}
