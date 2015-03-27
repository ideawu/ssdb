/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef SSDB_CLUSTER_MIGRATE_H_
#define SSDB_CLUSTER_MIGRATE_H_

#include "include.h"
#include <string>
#include "cluster.h"

namespace ssdb{
	class Client;
};

class ClusterMigrate
{
public:
	ClusterMigrate();
	~ClusterMigrate();
	
	// 完成后, src.range 和 dst.range 会被改变
	int64_t migrate_kv_data(Node *src, Node *dst, int num_keys);
	
private:
	int move_key(const std::string &key);
	int64_t move_range(const std::string &max_key, std::string *moved_max_key, int num_keys);

	int get_key_range(ssdb::Client *client, KeyRange *range);
	int set_key_range(ssdb::Client *client, const KeyRange &range);
	int check_version(ssdb::Client *client);
	
	ssdb::Client *src;
	ssdb::Client *dst;
};

#endif
