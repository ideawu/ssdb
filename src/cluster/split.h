#ifndef SSDB_CLUSTER_SPLIT_H
#define SSDB_CLUSTER_SPLIT_H

#include <string>
#include "ssdb_client.h"

class Split{
public:
	Split();
	~Split();
	
	int init(const std::string &cluster_ip, int cluster_port, const std::string &src_ip, int src_port, const std::string &dst_ip, int dst_port);
	// 返回迁移的数据的字节数(估计), -1 表示出错; 0 表示已迁移完毕.
	int64_t move_some();
	int finish();
	
private:
	int find_src_key_range_to_move(std::string *min_key, std::string *max_key);
	int64_t move_key_range(const std::string &min_key, const std::string &max_key);
	
	int move_key(const std::string &key);
	int copy_key(const std::string &key);
	
	int log_src_key(const std::string &key);
	int log_dst_key(const std::string &key);
	int set_src_key(const std::string &key);
	int set_dst_key(const std::string &key);
	int del_src_key(const std::string &key);

	std::string status_key;
	
	ssdb::Client *cluster;
	ssdb::Client *src_client;
	ssdb::Client *dst_client;
	
	std::string last_move_key;
};

#endif
