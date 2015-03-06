/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "version.h"
#include "../util/log.h"
#include "../util/strings.h"
#include "../net/proc.h"
#include "../net/server.h"
#include "serv.h"

DEF_PROC(info);
DEF_PROC(dbsize);
DEF_PROC(kv_node_list);
DEF_PROC(add_kv_node);
DEF_PROC(del_kv_node);
DEF_PROC(split_kv_node);
DEF_PROC(migrate_kv_data);

#define REG_PROC(c, f)     net->proc_map.set_proc(#c, f, proc_##c)

void ClusterServer::reg_procs(NetworkServer *net){
	REG_PROC(info, "w");
	REG_PROC(dbsize, "w");
	
	REG_PROC(kv_node_list, "w");
	REG_PROC(add_kv_node, "w");
	REG_PROC(del_kv_node, "w");
	REG_PROC(split_kv_node, "w");
	REG_PROC(migrate_kv_data, "w");
}

ClusterServer::ClusterServer(SSDB *ssdb, const Config &conf, NetworkServer *net){
	this->ssdb = (SSDBImpl *)ssdb;
	net->data = this;
	this->reg_procs(net);
	
	this->cluster = new Cluster();
	this->cluster->init(ssdb);
}

ClusterServer::~ClusterServer(){
	delete cluster;
}

int proc_kv_node_list(NetworkServer *net, Link *link, const Request &req, Response *resp){
	ClusterServer *serv = (ClusterServer *)net->data;
	resp->push_back("ok");
	std::map<std::string, Node *>::iterator it;
	std::map<std::string, Node *> &node_list = serv->cluster->kv_node_list;
	for(it = node_list.begin(); it != node_list.end(); it++){
		Node *node = it->second;
		resp->add(5);
		resp->add(node->id);
		resp->add(node->ip);
		resp->add(node->port);
		resp->add(node->kv_range.start);
		resp->add(node->kv_range.end);
	}
	return 0;
}

int proc_add_kv_node(NetworkServer *net, Link *link, const Request &req, Response *resp){
	ClusterServer *serv = (ClusterServer *)net->data;
	if(req.size() < 3){
		resp->add("client_error");
		return 0;
	}
		
	std::string ip = req[1].String();
	int port = req[2].Int();
	
	Node *node = serv->cluster->connect_node(ip, port);
	if(!node){
		log_error("connect to node %s:%d error!", ip.c_str(), port);
		resp->add("error");
		resp->add("connect to node error");
		return 0;
	}

	int ret;
	ret = serv->cluster->add_kv_node(node);
	resp->reply_status(ret);
	return 0;
}

int proc_del_kv_node(NetworkServer *net, Link *link, const Request &req, Response *resp){
	ClusterServer *serv = (ClusterServer *)net->data;
	if(req.size() < 2){
		resp->add("client_error");
		return 0;
	}

	int node_id = req[1].Int();
	Node *node = serv->cluster->get_node(node_id);
	if(!node){
		log_error("node %d not found!", node_id);
		resp->add("error");
		resp->add("node not found");
	}else{
		int ret = serv->cluster->del_kv_node(node);
		resp->reply_int(0, ret);
	}
	return 0;
}

int proc_split_kv_node(NetworkServer *net, Link *link, const Request &req, Response *resp){
	ClusterServer *serv = (ClusterServer *)net->data;
	if(req.size() < 4){
		resp->add("client_error");
		return 0;
	}
	
	int src_id = req[1].Int();
	std::string ip = req[2].String();
	int port = req[3].Int();
	
	Node *node1 = serv->cluster->get_node(src_id);
	if(!node1){
		log_error("source node %d not found!", src_id);
		resp->add("error");
		resp->add("source node not found");
		return 0;
	}
	
	Node *node2 = serv->cluster->connect_node(ip, port);
	if(!node2){
		log_error("connect to destination node %s:%d error!", ip.c_str(), port);
		resp->add("error");
		resp->add("connect to destination node error");
		return 0;
	}

	int ret;
	ret = serv->cluster->split_kv_node(node1, node2);
	if(ret > 0){
		resp->add("ok");
	}else{
		delete node2;
		resp->add("error");
	}
	return 0;
}

int proc_migrate_kv_data(NetworkServer *net, Link *link, const Request &req, Response *resp){
	ClusterServer *serv = (ClusterServer *)net->data;
	if(req.size() < 3){
		resp->add("client_error");
		return 0;
	}
	
	int src_id = req[1].Int();
	int dst_id = req[2].Int();
	Node *node1 = serv->cluster->get_node(src_id);
	Node *node2 = serv->cluster->get_node(dst_id);
	if(!node1){
		log_error("source node %d not found!", src_id);
		resp->add("error");
		resp->add("source node not found");
		return 0;
	}
	if(!node2){
		log_error("destination node %d not found!", dst_id);
		resp->add("error");
		resp->add("destination node not found");
		return 0;
	}

	int64_t size = serv->cluster->migrate_kv_data(node1, node2);
	if(size < 0){
		resp->add("error");
	}else{
		resp->reply_int(0, size);
	}
	return 0;
}

int proc_info(NetworkServer *net, Link *link, const Request &req, Response *resp){
	ClusterServer *serv = (ClusterServer *)net->data;
	resp->add("ok");
	resp->add("ssdb-cluster");
	resp->add("version");
	resp->add(SSDB_CLUSTER_VERSION);
	{
		resp->add("links");
		resp->add(net->link_count);
	}
	{
		int64_t calls = 0;
		proc_map_t::iterator it;
		for(it=net->proc_map.begin(); it!=net->proc_map.end(); it++){
			Command *cmd = it->second;
			calls += cmd->calls;
		}
		resp->add("total_calls");
		resp->add(calls);
	}
	
	{
		uint64_t size = serv->ssdb->size();
		resp->add("dbsize");
		resp->add(size);
	}

	{
		std::string s = serv->ssdb->binlogs->stats();
		resp->add("binlogs");
		resp->add(s);
	}

	return 0;
}

int proc_dbsize(NetworkServer *net, Link *link, const Request &req, Response *resp){
	ClusterServer *serv = (ClusterServer *)net->data;
	uint64_t size = serv->ssdb->size();
	resp->add("ok");
	resp->add(size);
	return 0;
}
