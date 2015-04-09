/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef UTIL__CONFIG_H
#define UTIL__CONFIG_H

/*
语法定义:
	空白字符为 '\t \r\n'(制表符, 空格, 回车, 换行)
	忽略只包含空白字符的行
	有效行以 '\t*' 开头
	注释行以 '\t*#' 开头
	key 和 value 之间可以用等号'='或者冒号':'分隔
	key 不包含任何空白字符, 两端的空白字符被忽略
	value 两端的空白字符被忽略
	配置项可以有包含关系, 用一个 TAB 缩进表示父子关系

配置读取:
	用键名获取子配置项
	用斜杠'/'或者句号'.'分隔的配置项路径获取配置项
	把配置项的值作为整形(int)返回
	把配置项的值作为字符串(char *)返回
*/

#include <string>
#include <vector>
#include <stdint.h>

#define CONFIG_MAX_LINE		4096

/* special filenames: stdin, stdout, stderr */
class Config{
	private:
		Config *parent;
		int depth;

		Config* build_key_path(const char *key);
		Config* add_child(const char *key, const char *val="", int lineno=0);
		const Config* find_child(const char *key) const;
	public:
		Config(const char *key=NULL, const char *val=NULL);
		~Config();

		static Config* load(const char *filename);
		int save(FILE *fp) const;
		int save(const char *filename) const;

		std::vector<Config *> children;
		std::string key;
		std::string val;

		Config* set(const char *key, const char *val);
		const Config* get(const char *key) const;
		int num() const;
		int get_num(const char *key) const;
		int64_t get_int64(const char *key) const;
		const char* str() const;
		const char* get_str(const char *key) const;

		bool is_comment() const{
			return key[0] == '#';
		}
		std::string ToString() const{
			return key + ": " + val;
		}
};

#endif

/*
配置文件示例:

# this is a comment

author : ideawu
	url: http://www.ideawu.net

proxy :
	php =
		host = 127.0.0.1
		port = 8088
	py :
		host = 127.0.0.1
		port = 8080

cgi =
	pl = /usr/bin/perl

应用程序示例:

#include <stdio.h>
#include "config.h"

int main(int argc, char **argv){
	struct config *cfg, *c;

	cfg = cfg_load_file("cfg_test.conf");
	if(!cfg){
		return 0;
	}

	printf("\n");
	printf("proxy.php.host = %s\n", cfg_getstr(cfg, "proxy.php.host"));
	printf("proxy.php.port = %d\n", cfg_getnum(cfg, "proxy.php.port"));
	printf("cgi.pl = %s\n", cfg_getstr(cfg, "cgi.pl"));
	printf("\n");

	c = cfg_get(cfg, "author");
	printf("author: %s\n", cfg_str(c));
	printf("url: %s\n", cfg_getstr(c, "url"));
	printf("\n");

	cfg_free(cfg);
	return 0;
}

*/
