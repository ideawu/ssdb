/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef SSDB_CLUSTER_H_
#define SSDB_CLUSTER_H_

#include "include.h"
#include <string>
#include <vector>
#include "util/strings.h"
#include "util/thread.h"

class KeyRange{
public:
	std::string begin;
	std::string end;
	KeyRange(){
	}
	KeyRange(const std::string &begin, const std::string &end){
		this->begin = begin;
		this->end = end;
	}
	std::string str() const{
		return "(\"" + str_escape(begin) + "\" - \"" + str_escape(end) + "\"]";
	}
	bool overlapped(const KeyRange &range) const{
		if(!this->begin.empty() && !range.end.empty() && this->begin >= range.end){
			return false;
		}
		if(!this->end.empty() && !range.begin.empty() && this->end <= range.begin){
			return false;
		}
		return true;
	}
	bool empty() const{
		return begin == "" && end == "";
	}
};

class Node{
public:
	const static int INIT    = 0;
	const static int SERVING = 1;
	
	int id;
	int status;
	KeyRange range;
	std::string ip;
	int port;
	
	Node(){
		this->id = 0;
		this->status = INIT;
	}
	std::string str() const{
		char buf[512];
		snprintf(buf, sizeof(buf), "%4d: %s", id, str_escape(range.str()).c_str());
		return std::string(buf);
	}
};

class SSDB;
class ClusterStore;

class Cluster
{
public:
	Cluster(SSDB *db);
	~Cluster();

	int init();
	
	int get_kv_node(int id, Node *ret);
	// 返回节点的 id
	int add_kv_node(const std::string &ip, int port);
	int del_kv_node(int id);
	int set_kv_range(int id, const KeyRange &range);
	int set_kv_status(int id, int status);
	int get_kv_node_list(std::vector<Node> *list);
	// 返回迁移的字节数
	int64_t migrate_kv_data(int src_id, int dst_id, int num_keys);

private:
	SSDB *db;
	ClusterStore *store;
	int next_id;
	std::vector<Node> kv_node_list;
	Mutex mutex;
	
	Node* get_kv_node_ref(int id);
};

#endif
