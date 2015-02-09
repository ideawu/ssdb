#ifndef SSDB_CLUSTER_SPLIT_H
#define SSDB_CLUSTER_SPLIT_H

#include <stdint.h>
#include <string>
#include "SSDB_client.h"

class Split{
public:
	Split();
	~Split();
	
	int init(const std::string &cluster_ip, int cluster_port, const std::string &src_ip, int src_port, const std::string &dst_ip, int dst_port);
	// 返回迁移的数据的字节数(估计), -1 表示出错; 0 表示已迁移完毕.
	int64_t move_some();
	int finish();

public:	
	ssdb::Client *src_client;
	ssdb::Client *dst_client;

private:
	int find_src_key_range_to_move(std::string *min_key, std::string *max_key);
	int64_t move_key_range(const std::string &min_key, const std::string &max_key);
	
	int copy_key(const std::string &key);
	
	int load_last_move_key();
	int save_last_move_key(const std::string &key);
	
	int set_src_kv_range(const std::string &min_key, const std::string &max_key);
	int set_dst_kv_range(const std::string &min_key, const std::string &max_key);
	int del_src_key(const std::string &key);

	ssdb::Client *cluster;
	std::string status_key;
	std::string last_move_key;
	std::string src_kv_range_s, src_kv_range_e;
	std::string dst_kv_range_s, dst_kv_range_e;
};

#endif
