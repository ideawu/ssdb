#include "SSDB_impl.h"
#include "util/strings.h"

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

/******************** KV *************************/

Status ClientImpl::get(const std::string &key, std::string *val){
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("get");
	req.push_back(key);
	resp = request(req);

	Status s(resp);
	if(s.ok()){
		if(resp->size() >= 2){
			*val = resp->at(1);
		}else{
			return Status("server_error");
		}
	}
	return s;
}

Status ClientImpl::set(const std::string &key, const std::string &val){
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("set");
	req.push_back(key);
	req.push_back(val);
	resp = request(req);

	Status s(resp);
	return s;
}

Status ClientImpl::del(const std::string &key){
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("del");
	req.push_back(key);
	resp = request(req);

	Status s(resp);
	return s;
}

Status ClientImpl::incr(const std::string &key, int64_t incrby, int64_t *ret){
	std::string s_incrby = int64_to_str(incrby);
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("incr");
	req.push_back(key);
	req.push_back(s_incrby);
	resp = request(req);

	Status s(resp);
	if(s.ok()){
		if(resp->size() >= 2){
			*ret = str_to_int64(resp->at(1));
		}else{
			return Status("server_error");
		}
	}
	return s;
}

Status ClientImpl::keys(const std::string &key_start, const std::string &key_end,
	int limit, std::vector<std::string> *ret)
{
	std::string s_limit = int_to_str(limit);
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("keys");
	req.push_back(key_start);
	req.push_back(key_end);
	req.push_back(s_limit);
	resp = request(req);

	Status s(resp);
	if(s.ok()){
		std::vector<std::string>::const_iterator it;
		for(it = resp->begin() + 1; it != resp->end(); it++){
			ret->push_back(*it);
		}
	}
	return s;
}

Status ClientImpl::scan(const std::string &key_start, const std::string &key_end,
	int limit, std::vector<std::string> *ret)
{
	std::string s_limit = int_to_str(limit);
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("scan");
	req.push_back(key_start);
	req.push_back(key_end);
	req.push_back(s_limit);
	resp = request(req);

	Status s(resp);
	if(s.ok()){
		std::vector<std::string>::const_iterator it;
		for(it = resp->begin() + 1; it != resp->end(); it++){
			ret->push_back(*it);
		}
	}
	return s;
}

Status ClientImpl::rscan(const std::string &key_start, const std::string &key_end,
	int limit, std::vector<std::string> *ret)
{
	std::string s_limit = int_to_str(limit);
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("rscan");
	req.push_back(key_start);
	req.push_back(key_end);
	req.push_back(s_limit);
	resp = request(req);

	Status s(resp);
	if(s.ok()){
		std::vector<std::string>::const_iterator it;
		for(it = resp->begin() + 1; it != resp->end(); it++){
			ret->push_back(*it);
		}
	}
	return s;
}


/******************** hash *************************/


Status ClientImpl::hget(const std::string &name, const std::string &key, std::string *val){
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("hget");
	req.push_back(name);
	req.push_back(key);
	resp = request(req);

	Status s(resp);
	if(s.ok()){
		if(resp->size() >= 2){
			*val = resp->at(1);
		}else{
			return Status("server_error");
		}
	}
	return s;
}

Status ClientImpl::hset(const std::string &name, const std::string &key, const std::string &val){
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("hset");
	req.push_back(name);
	req.push_back(key);
	req.push_back(val);
	resp = request(req);

	Status s(resp);
	return s;
}

Status ClientImpl::hdel(const std::string &name, const std::string &key){
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("hdel");
	req.push_back(name);
	req.push_back(key);
	resp = request(req);

	Status s(resp);
	return s;
}

Status ClientImpl::hincr(const std::string &name, const std::string &key, int64_t incrby, int64_t *ret){
	std::string s_incrby = int64_to_str(incrby);
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("hincr");
	req.push_back(name);
	req.push_back(key);
	req.push_back(s_incrby);
	resp = request(req);

	Status s(resp);
	if(s.ok()){
		if(resp->size() >= 2){
			*ret = str_to_int64(resp->at(1));
		}else{
			return Status("server_error");
		}
	}
	return s;
}

Status ClientImpl::hsize(const std::string &name, int64_t *ret){
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("hsize");
	req.push_back(name);
	resp = request(req);

	Status s(resp);
	if(s.ok()){
		if(resp->size() >= 2){
			*ret = str_to_int64(resp->at(1));
		}else{
			return Status("server_error");
		}
	}
	return s;
}

Status ClientImpl::hkeys(const std::string &name,
	const std::string &key_start, const std::string &key_end,
	int limit, std::vector<std::string> *ret)
{
	std::string s_limit = int_to_str(limit);
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("hkeys");
	req.push_back(name);
	req.push_back(key_start);
	req.push_back(key_end);
	req.push_back(s_limit);
	resp = request(req);

	Status s(resp);
	if(s.ok()){
		std::vector<std::string>::const_iterator it;
		for(it = resp->begin() + 1; it != resp->end(); it++){
			ret->push_back(*it);
		}
	}
	return s;
}

Status ClientImpl::hscan(const std::string &name,
	const std::string &key_start, const std::string &key_end,
	int limit, std::vector<std::string> *ret)
{
	std::string s_limit = int_to_str(limit);
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("hscan");
	req.push_back(name);
	req.push_back(key_start);
	req.push_back(key_end);
	req.push_back(s_limit);
	resp = request(req);

	Status s(resp);
	if(s.ok()){
		std::vector<std::string>::const_iterator it;
		for(it = resp->begin() + 1; it != resp->end(); it++){
			ret->push_back(*it);
		}
	}
	return s;
}

Status ClientImpl::hrscan(const std::string &name,
	const std::string &key_start, const std::string &key_end,
	int limit, std::vector<std::string> *ret)
{
	std::string s_limit = int_to_str(limit);
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("hrscan");
	req.push_back(name);
	req.push_back(key_start);
	req.push_back(key_end);
	req.push_back(s_limit);
	resp = request(req);

	Status s(resp);
	if(s.ok()){
		std::vector<std::string>::const_iterator it;
		for(it = resp->begin() + 1; it != resp->end(); it++){
			ret->push_back(*it);
		}
	}
	return s;
}


/******************** zset *************************/



Status ClientImpl::zget(const std::string &name, const std::string &key, int64_t *ret){
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("zget");
	req.push_back(name);
	req.push_back(key);
	resp = request(req);

	Status s(resp);
	if(s.ok()){
		if(resp->size() >= 2){
			*ret = str_to_int64(resp->at(1));
		}else{
			return Status("server_error");
		}
	}
	return s;
}

Status ClientImpl::zset(const std::string &name, const std::string &key, int64_t score){
	std::string s_score = int64_to_str(score);
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("zset");
	req.push_back(name);
	req.push_back(key);
	req.push_back(s_score);
	resp = request(req);

	Status s(resp);
	return s;
}

Status ClientImpl::zdel(const std::string &name, const std::string &key){
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("zdel");
	req.push_back(name);
	req.push_back(key);
	resp = request(req);

	Status s(resp);
	return s;
}

Status ClientImpl::zincr(const std::string &name, const std::string &key, int64_t incrby, int64_t *ret){
	std::string s_incrby = int64_to_str(incrby);
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("zincr");
	req.push_back(name);
	req.push_back(key);
	req.push_back(s_incrby);
	resp = request(req);

	Status s(resp);
	if(s.ok()){
		if(resp->size() >= 2){
			*ret = str_to_int64(resp->at(1));
		}else{
			return Status("server_error");
		}
	}
	return s;
}

Status ClientImpl::zsize(const std::string &name, int64_t *ret){
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("zsize");
	req.push_back(name);
	resp = request(req);

	Status s(resp);
	if(s.ok()){
		if(resp->size() >= 2){
			*ret = str_to_int64(resp->at(1));
		}else{
			return Status("server_error");
		}
	}
	return s;
}

Status ClientImpl::zkeys(const std::string &name, const std::string &key_start,
	int64_t *score_start, int64_t *score_end,
	int limit, std::vector<std::string> *ret)
{
	std::string s_score_start = score_start? int_to_str(*score_start) : "";
	std::string s_score_end = score_end? int_to_str(*score_end) : "";
	std::string s_limit = int_to_str(limit);
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("zkeys");
	req.push_back(name);
	req.push_back(key_start);
	req.push_back(s_score_start);
	req.push_back(s_score_end);
	req.push_back(s_limit);
	resp = request(req);

	Status s(resp);
	if(s.ok()){
		std::vector<std::string>::const_iterator it;
		for(it = resp->begin() + 1; it != resp->end(); it++){
			ret->push_back(*it);
		}
	}
	return s;
}

Status ClientImpl::zscan(const std::string &name, const std::string &key_start,
	int64_t *score_start, int64_t *score_end,
	int limit, std::vector<std::string> *ret)
{
	std::string s_score_start = score_start? int_to_str(*score_start) : "";
	std::string s_score_end = score_end? int_to_str(*score_end) : "";
	std::string s_limit = int_to_str(limit);
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("zscan");
	req.push_back(name);
	req.push_back(key_start);
	req.push_back(s_score_start);
	req.push_back(s_score_end);
	req.push_back(s_limit);
	resp = request(req);

	Status s(resp);
	if(s.ok()){
		std::vector<std::string>::const_iterator it;
		for(it = resp->begin() + 1; it != resp->end(); it++){
			ret->push_back(*it);
		}
	}
	return s;
}

Status ClientImpl::zrscan(const std::string &name, const std::string &key_start,
	int64_t *score_start, int64_t *score_end,
	int limit, std::vector<std::string> *ret)
{
	std::string s_score_start = score_start? int_to_str(*score_start) : "";
	std::string s_score_end = score_end? int_to_str(*score_end) : "";
	std::string s_limit = int_to_str(limit);
	std::vector<std::string> req;
	const std::vector<std::string> *resp;
	req.push_back("zrscan");
	req.push_back(name);
	req.push_back(key_start);
	req.push_back(s_score_start);
	req.push_back(s_score_end);
	req.push_back(s_limit);
	resp = request(req);

	Status s(resp);
	if(s.ok()){
		std::vector<std::string>::const_iterator it;
		for(it = resp->begin() + 1; it != resp->end(); it++){
			ret->push_back(*it);
		}
	}
	return s;
}

}; // namespace ssdb
