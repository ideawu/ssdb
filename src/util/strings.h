#ifndef UTIL_STRING_H
#define UTIL_STRING_H

#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string>

inline static
int is_empty_str(const char *str){
	const char *p = str;
	while(*p && isspace(*p)){
		p++;
	}
	return *p == '\0';
}

/* 返回左边不包含空白字符的字符串的指针 */
inline static
char *ltrim(const char *str){
	const char *p = str;
	while(*p && isspace(*p)){
		p++;
	}
	return (char *)p;
}

/* 返回指向字符串结尾的指针, 会修改字符串内容 */
inline static
char *rtrim(char *str){
	char *p;
	p = str + strlen(str) - 1;
	while(p >= str && isspace(*p)){
		p--;
	}
	*(++p) = '\0';
	return p;
}

/* 返回左边不包含空白字符的字符串的指针 */
inline static
char *trim(char *str){
	char *p;
	p = ltrim(str);
	rtrim(p);
	return p;
}

inline static
std::string real_dirname(const char *filepath){
	std::string dir;
	if(filepath[0] != '/'){
		char buf[1024];
		char *p = getcwd(buf, sizeof(buf));
		if(p != NULL){
			dir.append(p);
		}
	}

	const char *p = strrchr(filepath, '/');
	if(p != NULL){
		dir.append("/");
		dir.append(filepath, p - filepath);
	}
	return dir;
}

inline static
std::string hexmem(const void *p, int size){
	std::string ret;
	char buf[4];
	for(int i=0; i<size; i++){
		char c = ((char *)p)[i];
		if(isalnum(c) || isprint(c)){
			ret.append(1, c);
		}else{
			switch(c){
				case '\r':
					ret.append("\\r", 2);
					break;
				case '\n':
					ret.append("\\n", 2);
					break;
				default:
					sprintf(buf, "\\%02x", (unsigned char)c);
					ret.append(buf, 3);
			}
		}
	}
	return ret;
}

// TODO: mem_printf("%5c%d%s", p, size);
static inline
void dump(const void *p, int size, const char *msg = NULL){
	if(msg == NULL){
		printf("dump <");
	}else{
		printf("%s <", msg);
	}
	std::string s = hexmem(p, size);
	printf("%s>\n", s.c_str());
}

static inline
int str_to_int(const char *p, int size){
	return atoi(std::string(p, size).c_str());
}

static inline
std::string int_to_str(int v){
	char buf[21] = {0};
	snprintf(buf, sizeof(buf), "%d", v);
	return std::string(buf);
}

static inline
int64_t str_to_int64(const char *p, int size){
	return atoll(std::string(p, size).c_str());
}

static inline
std::string int64_to_str(int64_t v){
	char buf[21] = {0};
	snprintf(buf, sizeof(buf), "%lld", (long long int)v);
	return std::string(buf);
}

static inline
int64_t str_to_uint64(const char *p, int size){
	return strtol(std::string(p, size).c_str(), (char **)NULL, 10);
}

static inline
std::string uint64_to_str(uint64_t v){
	char buf[21] = {0};
	snprintf(buf, sizeof(buf), "%llu", (long long unsigned int)v);
	return std::string(buf);
}

static inline
double str_to_double(const char *p, int size){
	return atof(std::string(p, size).c_str());
}

static inline
std::string double_to_str(double v){
	char buf[21] = {0};
	if(v - floor(v) == 0){
		snprintf(buf, sizeof(buf), "%.0f", v);
	}else{
		snprintf(buf, sizeof(buf), "%f", v);
	}
	return std::string(buf);
}

/**/

// is big endia. TODO: auto detect
#if 0
	#define big_endian(v) (v)
#else
	static inline
	uint16_t big_endian(uint16_t v){
		return (v>>8) | (v<<8);
	}

	static inline
	uint32_t big_endian(uint32_t v){
		return (v >> 24) | ((v >> 8) & 0xff00) | ((v << 8) & 0xff0000) | (v << 24);
	}

	static inline
	uint64_t big_endian(uint64_t v){
		uint32_t h = v >> 32;
		uint32_t l = v & 0xffffffffull;
		return big_endian(h) | ((uint64_t)big_endian(l) << 32);
	}
#endif


#endif
