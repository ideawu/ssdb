/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
/* hash */
#include "serv.h"
#include "net/proc.h"
#include "net/server.h"

int proc_cluster_kv_node_list(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	Cluster *cluster = serv->cluster;
	std::vector<Node> node_list;
	int ret = cluster->get_kv_node_list(&node_list);
	if(ret == -1){
		resp->add("error");
	}else{
		resp->add("ok");
		std::vector<Node>::iterator it;
		for(it=node_list.begin(); it!=node_list.end(); it++){
			const Node &node = *it;
			resp->add(6);
			resp->add(node.id);
			resp->add(node.status);
			resp->add(node.range.begin);
			resp->add(node.range.end);
			resp->add(node.ip);
			resp->add(node.port);
		}
	}
	return 0;
}

int proc_cluster_add_kv_node(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	Cluster *cluster = serv->cluster;
	if(req.size() < 3){
		resp->add("client_error");
		return 0;
	}
	
	std::string ip = req[1].String();
	int port = req[2].Int();
	int id = cluster->add_kv_node(ip, port);
	if(id == -1){
		resp->add("error");
	}else{
		resp->reply_int(0, id);
	}
	return 0;
}

int proc_cluster_del_kv_node(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	Cluster *cluster = serv->cluster;
	if(req.size() < 2){
		resp->add("client_error");
		return 0;
	}
	
	int id = req[1].Int();
	int ret = cluster->del_kv_node(id);
	if(ret == -1){
		resp->add("error");
	}else{
		resp->reply_int(0, ret);
	}
	return 0;
}

int proc_cluster_set_kv_range(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	Cluster *cluster = serv->cluster;
	if(req.size() < 4){
		resp->add("client_error");
		return 0;
	}
	
	int id = req[1].Int();
	std::string begin = req[2].String();
	std::string end = req[3].String();
	int ret = cluster->set_kv_range(id, KeyRange(begin, end));
	if(ret == -1){
		resp->add("error");
	}else{
		resp->reply_int(0, ret);
	}
	return 0;
}

int proc_cluster_set_kv_status(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	Cluster *cluster = serv->cluster;
	if(req.size() < 3){
		resp->add("client_error");
		return 0;
	}
	
	int id = req[1].Int();
	int status = req[2].Int();
	if(status != Node::INIT && status != Node::SERVING){
		resp->add("client_error");
		resp->add("invalid status");
		return 0;
	}
	int ret = cluster->set_kv_status(id, status);
	if(ret == -1){
		resp->add("error");
	}else{
		resp->reply_int(0, ret);
	}
	return 0;
}

int proc_cluster_migrate_kv_data(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	Cluster *cluster = serv->cluster;
	if(req.size() < 4){
		resp->push_back("client_error");
		return 0;
	}

	int src_id = req[1].Int();
	int dst_id = req[2].Int();
	int num_keys = req[3].Int();
	int64_t ret = cluster->migrate_kv_data(src_id, dst_id, num_keys);
	if(ret == -1){
		resp->add("error");
	}else{
		resp->reply_int(0, ret);
	}
	return 0;
}
