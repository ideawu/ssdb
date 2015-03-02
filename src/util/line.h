#ifndef UTIL_LINE_H
#define UTIL_LINE_H

#include <inttypes.h>
#include <string>
#include "strings.h"

class LineEncoder{
public:
	int write(const std::string &data){
		val.append(str_escape(data));
		val.append("\n");
		return 0;
	}
	
	int write(int data){
		return this->write(::str(data));
	}
	
	int write(int64_t data){
		return this->write(::str(data));
	}
	
	std::string str(){
		return val;
	}
private:
	std::string val;
};

class LineDecoder{
public:
	LineDecoder(const std::string &s){
		spos = 0;
		epos = 0;
		buf = s.data();
		len = (int)s.size();
	}
	
	int readline(std::string *ret){
		return this->read(ret);
	}
	
	int read(std::string *ret){
		while(epos < len && buf[epos] != '\n'){
			epos ++;
		}
		if(epos >= len || buf[epos] != '\n'){
			return -1;
		}
		std::string line(&buf[spos], epos - spos);
		spos = epos + 1;
		epos = spos;
		*ret = str_unescape(line);
		return (int)ret->size();
	}
	
	int read(int *ret){
		std::string line;
		if(this->read(&line) == -1){
			return -1;
		}
		*ret = str_to_int(line);
		return 0;
	}
		
	int read(int64_t *ret){
		std::string line;
		if(this->read(&line) == -1){
			return -1;
		}
		*ret = str_to_int64(line);
		return 0;
	}
	
public:
	const char *buf;
	int len;
	int spos;
	int epos;
};

#endif
