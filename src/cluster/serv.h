/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef SSDB_CLUSTER_SERVER_H_
#define SSDB_CLUSTER_SERVER_H_

#include "include.h"
#include <map>
#include <vector>
#include <string>
#include "../ssdb/ssdb_impl.h"
#include "../net/server.h"
#include "cluster.h"

class ClusterServer
{
private:
	void reg_procs(NetworkServer *net);

public:
	SSDBImpl *ssdb;
	Cluster *cluster;

	ClusterServer(SSDB *ssdb, const Config &conf, NetworkServer *net);
	~ClusterServer();
};

#endif
