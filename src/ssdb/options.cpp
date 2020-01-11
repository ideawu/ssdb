/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "options.h"
#include "../util/string_util.h"

#ifdef NDEBUG
	static const int LOG_QUEUE_SIZE  = 20 * 1000 * 1000;
#else
	static const int LOG_QUEUE_SIZE  = 10000;
#endif

Options::Options(){
	Config c;
	this->load(c);
}

void Options::load(const Config &conf){
	cache_size = (size_t)conf.get_num("leveldb.cache_size");
	max_open_files = (size_t)conf.get_num("leveldb.max_open_files");
	write_buffer_size = (size_t)conf.get_num("leveldb.write_buffer_size");
	block_size = (size_t)conf.get_num("leveldb.block_size");
	compaction_speed = conf.get_num("leveldb.compaction_speed");
	compression = conf.get_str("leveldb.compression");
	std::string binlog = conf.get_str("replication.binlog");
	binlog_capacity = (size_t)conf.get_num("replication.binlog.capacity");

	strtolower(&compression);
	if(compression != "no"){
		compression = "yes";
	}
	strtolower(&binlog);
	if(binlog != "yes"){
		this->binlog = false;
	}else{
		this->binlog = true;
	}
	if(binlog_capacity <= 0){
		binlog_capacity = LOG_QUEUE_SIZE;
	}

	if(cache_size <= 0){
		cache_size = 16;
	}
	if(write_buffer_size <= 0){
		write_buffer_size = 16;
	}
	if(block_size <= 0){
		block_size = 32;
	}
	if(max_open_files <= 0){
		max_open_files = cache_size / 1024 * 300;
		if(max_open_files < 500){
			max_open_files = 500;
		}
		if(max_open_files > 1000){
			max_open_files = 1000;
		}
	}
}
