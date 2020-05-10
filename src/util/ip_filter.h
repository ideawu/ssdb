/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef ICOMET_IPFILTER_H
#define ICOMET_IPFILTER_H

#include <string>
#include <set>

// filter ip address
class IpFilter{
private:
	
	bool check_hit(const std::set<std::string> &m, const std::string &ip){
		if(m.empty()){
			return false;
		}
		std::set<std::string>::const_iterator it;
		it = m.upper_bound(ip);
		if(it == m.end()){
			return false;
		}
		const std::string &prefix = *it;

		int len = prefix.size() - 1;
		if(prefix[len] == '='){
			return prefix.compare(0, len, ip) == 0;
		}else if(ip.size() >= len){
			return ip.compare(0, len, prefix, 0, len) == 0;
		}
		return false;
	}
	
	bool is_full_ip(const std::string &ip_prefix){
		int n = 0;
		for(int i=0; i<(int)ip_prefix.size(); i++){
			if(ip_prefix[i] == '.'){
				n ++;
			}
		}
		return n == 3;
	}

public:
	bool allow_all;
	bool deny_all;
	std::set<std::string> deny;
	std::set<std::string> allow;

	IpFilter(){
		allow_all = true;
		deny_all = false;
	}
	
	void add_allow(const std::string &ip_prefix){
		if(ip_prefix == "all" || ip_prefix == "*"){
			allow_all = true;
		}else{
			allow_all = false;
			// '@' and '=' is greater than any char in ip
			std::string prefix = ip_prefix + (is_full_ip(ip_prefix)? "=" : "@");
			allow.insert(prefix);
		}
	}

	void del_allow(const std::string &ip_prefix){
		if(ip_prefix == "all" || ip_prefix == "*"){
			allow_all = false;
		}else{
			std::string prefix = ip_prefix + (is_full_ip(ip_prefix)? "=" : "@");
			allow.erase(prefix);
		}
	}
	
	void add_deny(const std::string &ip_prefix){
		if(ip_prefix == "all" || ip_prefix == "*"){
			deny_all = true;
		}else{
			// deny_all is always true
			// '@' and '=' is greater than any char in ip
			std::string prefix = ip_prefix + (is_full_ip(ip_prefix)? "=" : "@");
			deny.insert(prefix);
		}
	}

	void del_deny(const std::string &ip_prefix){
		if(ip_prefix == "all" || ip_prefix == "*"){
			deny_all = false;
		}else{
			std::string prefix = ip_prefix + (is_full_ip(ip_prefix)? "=" : "@");
			deny.erase(prefix);
		}
	}
	
	bool check_pass(const std::string &ip){
		// check specified allow/deny
		if(check_hit(deny, ip)){
			return false;
		}
		if(check_hit(allow, ip)){
			return true;
		}
		if(deny_all){
			return false;
		}
		if(allow_all){
			return true;
		}else{
			return false;
		}
	}
};

#endif
