#include "node.h"
#include "../util/log.h"

Node::Node(){
	this->id = 0;
	this->db = NULL;
}

Node::~Node(){
	delete db;
}

int Node::init(const std::string &ip, int port){
	this->db = ssdb::Client::connect(ip, port);
	if(this->db == NULL){
		log_error("failed to connect to cluster server!");
		return -1;
	}

	const std::vector<std::string>* resp;
	resp = this->db->request("ignore_key_range");
	if(!resp || resp->empty() || resp->at(0) != "ok"){
		log_error("src server ignore_key_range error!");
		return -1;
	}
	resp = this->db->request("get_kv_range");
	if(!resp || resp->size() != 3 || resp->at(0) != "ok"){
		log_error("src server get_kv_range error!");
		return -1;
	}
	kv_range.start = resp->at(1);
	kv_range.end = resp->at(2);
	
	return 0;
}
