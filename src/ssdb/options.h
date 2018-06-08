/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef SSDB_OPTION_H_
#define SSDB_OPTION_H_

#include "../util/config.h"

class Options
{
public:
    Options();
	~Options(){}
	
	void load(const Config &conf);

	size_t cache_size;
	size_t max_open_files;
	size_t write_buffer_size;
	size_t block_size;
	int compaction_speed;
    bool compression;
	bool binlog;
	size_t binlog_capacity;
	size_t bloom_filter_policy_size;
	bool create_if_missing = false;    // default for leveldb
	bool error_if_exists = false;      // default for leveldb
	bool paranoid_checks = false;      // default for leveldb
	size_t block_restart_interval;
	size_t max_file_size;
	bool reuse_logs = false;           // default to leveldb
};

#endif
