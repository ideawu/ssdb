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
	int ret = cluster->get_node_list(&node_list);
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
			resp->add(node.ip);
			resp->add(node.port);
			resp->add(node.range.begin);
			resp->add(node.range.end);
		}
	}
	return 0;
}

int proc_cluster_add_kv_node(NetworkServer *net, Link *link, const Request &req, Response *resp){
	SSDBServer *serv = (SSDBServer *)net->data;
	Cluster *cluster = serv->cluster;
	int id = cluster->add_node("127.0.0.1", 8888);
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
		resp->push_back("client_error");
		return 0;
	}
	int id = req[1].Int();
	int ret = cluster->del_node(id);
	if(ret == -1){
		resp->add("error");
	}else{
		resp->reply_int(0, ret);
	}
	return 0;
}
