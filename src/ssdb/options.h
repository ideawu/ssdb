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
	std::string compression;
	bool binlog;
};

#endif
