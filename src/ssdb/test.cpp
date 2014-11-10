/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include <string>
#include "ssdb.h"
#include "../util/log.h"
#include "../util/config.h"

int main(int argc, char **argv){
	set_log_level(Logger::LEVEL_TRACE);
	std::string work_dir = "./tmp/a";
	Options opt;
	opt.compression = "no";

	SSDB *ssdb = NULL;
	ssdb = SSDB::open(opt, work_dir);
	if(!ssdb){
		log_fatal("could not open work_dir: %s", work_dir.c_str());
		fprintf(stderr, "could not open work_dir: %s\n", work_dir.c_str());
		exit(1);
	}
	std::string key, val;
	key = "a";
	
	val.append(1024 * 1024, 'a');
	ssdb->raw_set("tmp", val);
	ssdb->compact();

	uint64_t size;
	size = ssdb->size();
	log_debug("dbsize: %d", size);


	ssdb->get(key, &val);
	int num = str_to_int(val) + 1;
	ssdb->set(key, str(num));
	ssdb->get(key, &val);
	
	log_debug("%s", val.c_str());
	delete ssdb;
}
