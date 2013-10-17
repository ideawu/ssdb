#include "SSDB_impl.h"

namespace ssdb{

ClientImpl::ClientImpl(){
	link = NULL;
}

ClientImpl::~ClientImpl(){
	if(link){
		delete link;
	}
}

Client* Client::connect(const char *ip, int port){
	ClientImpl *client = new ClientImpl();
	client->link = Link::connect(ip, port);
	if(client->link == NULL){
		return NULL;
	}
	return client;
}

const std::vector<std::string>* ClientImpl::request(const std::vector<std::string> &req){
	if(link->send(req) == -1){
		return NULL;
	}
	if(link->flush() == -1){
		return NULL;
	}
	const std::vector<Bytes> *packet = link->response();
	if(packet == NULL){
		return NULL;
	}
	resp_.clear();
	for(std::vector<Bytes>::const_iterator it=packet->begin(); it!=packet->end(); it++){
		const Bytes &b = *it;
		resp_.push_back(b.String());
	}
	return &resp_;
}

const std::vector<std::string>* ClientImpl::request(const std::string &cmd){
	std::vector<std::string> req;
	req.push_back(cmd);
	return request(req);
}

const std::vector<std::string>* ClientImpl::request(const std::string &cmd, const std::string &s2){
	std::vector<std::string> req;
	req.push_back(cmd);
	req.push_back(s2);
	return request(req);
}

const std::vector<std::string>* ClientImpl::request(const std::string &cmd, const std::string &s2, const std::string &s3){
	std::vector<std::string> req;
	req.push_back(cmd);
	req.push_back(s2);
	req.push_back(s3);
	return request(req);
}

const std::vector<std::string>* ClientImpl::request(const std::string &cmd, const std::string &s2, const std::string &s3, const std::string &s4){
	std::vector<std::string> req;
	req.push_back(cmd);
	req.push_back(s2);
	req.push_back(s3);
	req.push_back(s4);
	return request(req);
}

const std::vector<std::string>* ClientImpl::request(const std::string &cmd, const std::string &s2, const std::string &s3, const std::string &s4, const std::string &s5){
	std::vector<std::string> req;
	req.push_back(cmd);
	req.push_back(s2);
	req.push_back(s3);
	req.push_back(s4);
	req.push_back(s5);
	return request(req);
}

Status ClientImpl::get(const std::string &key, std::string *val){
	Status s;
	return s;
}

Status ClientImpl::set(const std::string &key, const std::string &val){
	Status s;
	return s;
}

Status ClientImpl::del(const std::string &key){
	Status s;
	return s;
}

Status ClientImpl::incr(const std::string &key, int64_t incrby, int64_t *ret){
	Status s;
	return s;
}

Status ClientImpl::keys(const std::string &key_start, const std::string &key_end,
	int limit, std::vector<std::string> *ret)
{
	Status s;
	return s;
}

Status ClientImpl::scan(const std::string &key_start, const std::string &key_end,
	int limit, std::vector<std::string> *ret)
{
	Status s;
	return s;
}

Status ClientImpl::rscan(const std::string &key_start, const std::string &key_end,
	int limit, std::vector<std::string> *ret)
{
	Status s;
	return s;
}

}; // namespace ssdb
