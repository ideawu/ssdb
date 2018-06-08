/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "options.h"
#include "../util/strings.h"

#include <iostream>

#ifdef NDEBUG
	static const int LOG_QUEUE_SIZE  = 20 * 1000 * 1000;
#else
	static const int LOG_QUEUE_SIZE  = 10000;
#endif

Options::Options(){
	Config c;
    load(c);
}

void Options::load(const Config &conf){
	cache_size = (size_t)conf.get_num("leveldb.cache_size");
	max_open_files = (size_t)conf.get_num("leveldb.max_open_files");
	write_buffer_size = (size_t)conf.get_num("leveldb.write_buffer_size");
	block_size = (size_t)conf.get_num("leveldb.block_size");
	compaction_speed = conf.get_num("leveldb.compaction_speed");
    std::string compression_str = conf.get_str("leveldb.compression");
	bloom_filter_policy_size = (size_t)conf.get_num("leveldb.bloom_filter_policy_size");
    std::string binlog_str = conf.get_str("replication.binlog");
	binlog_capacity = (size_t)conf.get_num("replication.binlog.capacity");
	std::string create_str = conf.get_str("leveldb.create_if_missing");
	std::string exists_str = conf.get_str("leveldb.error_if_exists");
	std::string checks_str = conf.get_str("leveldb.paranoid_checks");
	block_restart_interval = conf.get_num("leveldb.block_restart_interval");
	max_file_size = conf.get_num("leveldb.max_file_size");
    std::string reuse_str = conf.get_str("leveldb.reuse_logs");
	
    strtolower(&compression_str);
    if(compression_str != "no"){
        compression = true;
    }else{
        compression = false;
    }

    strtolower(&binlog_str);
    if(binlog_str != "yes"){
        binlog = false;
	}else{
        binlog = true;
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

	if(bloom_filter_policy_size <= 0){
		bloom_filter_policy_size = 10;
	}

    strtolower(&create_str);
    if(create_str == "yes"){
        create_if_missing = true;
    }
    strtolower(&exists_str);
    if(exists_str == "yes"){
        error_if_exists = true;
    }

    strtolower(&checks_str);
    if(checks_str == "yes"){
        paranoid_checks = true;
    }

	if(block_restart_interval <= 0){
        block_restart_interval = 16;
	}

	if(max_file_size <= 0){
		max_file_size = 32 * 1048576; // leveldb 1.20
	}

    strtolower(&reuse_str);
    if(reuse_str == "yes"){
        reuse_logs = true;
    }
}
