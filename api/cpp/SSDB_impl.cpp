#include "SSDB_impl.h"
#include "util/strings.h"

namespace ssdb{

inline static
Status _read_list(const std::vector<std::string> *resp, std::vector<std::string> *ret){
	Status s(resp);
	if(s.ok()){
		std::vector<std::string>::const_iterator it;
		for(it = resp->begin() + 1; it != resp->end(); it++){
			ret->push_back(*it);
		}
	}
	return s;
}

inline static
Status _read_int64(const std::vector<std::string> *resp, int64_t *ret){
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

inline static
Status _read_str(const std::vector<std::string> *resp, std::string *ret){
	Status s(resp);
	if(s.ok()){
		if(resp->size() >= 2){
			*ret = resp->at(1);
		}else{
			return Status("server_error");
		}
	}
	return s;
}

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

const std::vector<std::string>* ClientImpl::request(const std::string &cmd, const std::string &s2, const std::string &s3, const std::string &s4, const std::string &s5, const std::string &s6){
	std::vector<std::string> req;
	req.push_back(cmd);
	req.push_back(s2);
	req.push_back(s3);
	req.push_back(s4);
	req.push_back(s5);
	req.push_back(s6);
	return request(req);
}

/******************** KV *************************/

Status ClientImpl::get(const std::string &key, std::string *val){
	const std::vector<std::string> *resp;
	resp = this->request("get", key);
	return _read_str(resp, val);
}

Status ClientImpl::set(const std::string &key, const std::string &val){
	const std::vector<std::string> *resp;
	resp = this->request("set", key, val);
	Status s(resp);
	return s;
}

Status ClientImpl::setx(const std::string &key, const std::string &val, int ttl){
	const std::vector<std::string> *resp;
	resp = this->request("setx", key, val, int_to_str(ttl));
	Status s(resp);
	return s;
}

Status ClientImpl::del(const std::string &key){
	const std::vector<std::string> *resp;
	resp = this->request("del", key);
	Status s(resp);
	return s;
}

Status ClientImpl::incr(const std::string &key, int64_t incrby, int64_t *ret){
	std::string s_incrby = int64_to_str(incrby);
	const std::vector<std::string> *resp;
	resp = this->request("incr", key, s_incrby);
	return _read_int64(resp, ret);
}

Status ClientImpl::keys(const std::string &key_start, const std::string &key_end,
	uint64_t limit, std::vector<std::string> *ret)
{
	std::string s_limit = uint64_to_str(limit);
	const std::vector<std::string> *resp;
	resp = this->request("keys", key_start, key_end, s_limit);
	return _read_list(resp, ret);
}

Status ClientImpl::scan(const std::string &key_start, const std::string &key_end,
	uint64_t limit, std::vector<std::string> *ret)
{
	std::string s_limit = uint64_to_str(limit);
	const std::vector<std::string> *resp;
	resp = this->request("scan", key_start, key_end, s_limit);
	return _read_list(resp, ret);
}

Status ClientImpl::rscan(const std::string &key_start, const std::string &key_end,
	uint64_t limit, std::vector<std::string> *ret)
{
	std::string s_limit = uint64_to_str(limit);
	const std::vector<std::string> *resp;
	resp = this->request("rscan", key_start, key_end, s_limit);
	return _read_list(resp, ret);
}

/******************** queue *************************/

Status ClientImpl::qoffer(const std::string &name, const std::string &val){
	const std::vector<std::string> *resp;
	resp = this->request("qoffer", name, val);
	Status s(resp);
	return s;
}
Status ClientImpl::qpoll(const std::string &name, std::string *val){
	const std::vector<std::string> *resp;
	resp = this->request("qpoll", name);
	return _read_str(resp, val);
}
Status ClientImpl::qpeek(const std::string &name, std::string *val){
	const std::vector<std::string> *resp;
	resp = this->request("qpeek", name);
	return _read_str(resp, val);
}
Status ClientImpl::qsize(const std::string &name, int64_t *ret){
	const std::vector<std::string> *resp;
	resp = this->request("qsize", name);
	return _read_int64(resp, ret);
}

/******************** hash *************************/


Status ClientImpl::hget(const std::string &name, const std::string &key, std::string *val){
	const std::vector<std::string> *resp;
	resp = this->request("hget", name, key);
	return _read_str(resp, val);
}

Status ClientImpl::hset(const std::string &name, const std::string &key, const std::string &val){
	const std::vector<std::string> *resp;
	resp = this->request("hset", name, key, val);
	Status s(resp);
	return s;
}

Status ClientImpl::hdel(const std::string &name, const std::string &key){
	const std::vector<std::string> *resp;
	resp = this->request("hdel", name, key);
	Status s(resp);
	return s;
}

Status ClientImpl::hincr(const std::string &name, const std::string &key, int64_t incrby, int64_t *ret){
	std::string s_incrby = int64_to_str(incrby);
	const std::vector<std::string> *resp;
	resp = this->request("hincr", name, key, s_incrby);
	return _read_int64(resp, ret);
}

Status ClientImpl::hsize(const std::string &name, int64_t *ret){
	const std::vector<std::string> *resp;
	resp = this->request("hsize", name);
	return _read_int64(resp, ret);
}

Status ClientImpl::hclear(const std::string &name, int64_t *ret){
	const std::vector<std::string> *resp;
	resp = this->request("hclear", name);
	return _read_int64(resp, ret);
}

Status ClientImpl::hkeys(const std::string &name,
	const std::string &key_start, const std::string &key_end,
	uint64_t limit, std::vector<std::string> *ret)
{
	std::string s_limit = uint64_to_str(limit);
	const std::vector<std::string> *resp;
	resp = this->request("hkeys", name, key_start, key_end, s_limit);
	return _read_list(resp, ret);
}

Status ClientImpl::hscan(const std::string &name,
	const std::string &key_start, const std::string &key_end,
	uint64_t limit, std::vector<std::string> *ret)
{
	std::string s_limit = uint64_to_str(limit);
	const std::vector<std::string> *resp;
	resp = this->request("hscan", name, key_start, key_end, s_limit);
	return _read_list(resp, ret);
}

Status ClientImpl::hrscan(const std::string &name,
	const std::string &key_start, const std::string &key_end,
	uint64_t limit, std::vector<std::string> *ret)
{
	std::string s_limit = uint64_to_str(limit);
	const std::vector<std::string> *resp;
	resp = this->request("hrscan", name, key_start, key_end, s_limit);
	return _read_list(resp, ret);
}


/******************** zset *************************/



Status ClientImpl::zget(const std::string &name, const std::string &key, int64_t *ret){
	const std::vector<std::string> *resp;
	resp = this->request("zget", name, key);
	return _read_int64(resp, ret);
}

Status ClientImpl::zset(const std::string &name, const std::string &key, int64_t score){
	std::string s_score = int64_to_str(score);
	const std::vector<std::string> *resp;
	resp = this->request("zset", name, key, s_score);
	Status s(resp);
	return s;
}

Status ClientImpl::zdel(const std::string &name, const std::string &key){
	const std::vector<std::string> *resp;
	resp = this->request("zdel", name, key);
	Status s(resp);
	return s;
}

Status ClientImpl::zincr(const std::string &name, const std::string &key, int64_t incrby, int64_t *ret){
	std::string s_incrby = int64_to_str(incrby);
	const std::vector<std::string> *resp;
	resp = this->request("zincr", name, key, s_incrby);
	return _read_int64(resp, ret);
}

Status ClientImpl::zsize(const std::string &name, int64_t *ret){
	const std::vector<std::string> *resp;
	resp = this->request("zsize", name);
	return _read_int64(resp, ret);
}

Status ClientImpl::zclear(const std::string &name, int64_t *ret){
	const std::vector<std::string> *resp;
	resp = this->request("zclear", name);
	return _read_int64(resp, ret);
}

Status ClientImpl::zrank(const std::string &name, const std::string &key, int64_t *ret){
	const std::vector<std::string> *resp;
	resp = this->request("zrank", name, key);
	return _read_int64(resp, ret);
}

Status ClientImpl::zrrank(const std::string &name, const std::string &key, int64_t *ret){
	const std::vector<std::string> *resp;
	resp = this->request("zrrank", name, key);
	return _read_int64(resp, ret);
}

Status ClientImpl::zrange(const std::string &name,
		uint64_t offset, uint64_t limit,
		std::vector<std::string> *ret)
{
	std::string s_offset = uint64_to_str(offset);
	std::string s_limit = uint64_to_str(limit);
	const std::vector<std::string> *resp;
	resp = this->request("zrange", name, s_offset, s_limit);
	return _read_list(resp, ret);
}

Status ClientImpl::zrrange(const std::string &name,
		uint64_t offset, uint64_t limit,
		std::vector<std::string> *ret)
{
	std::string s_offset = uint64_to_str(offset);
	std::string s_limit = uint64_to_str(limit);
	const std::vector<std::string> *resp;
	resp = this->request("zrrange", name, s_offset, s_limit);
	return _read_list(resp, ret);
}

Status ClientImpl::zkeys(const std::string &name, const std::string &key_start,
	int64_t *score_start, int64_t *score_end,
	uint64_t limit, std::vector<std::string> *ret)
{
	std::string s_score_start = score_start? int_to_str(*score_start) : "";
	std::string s_score_end = score_end? int_to_str(*score_end) : "";
	std::string s_limit = uint64_to_str(limit);
	const std::vector<std::string> *resp;
	resp = this->request("zkeys", name, key_start, s_score_start, s_score_end, s_limit);
	return _read_list(resp, ret);
}

Status ClientImpl::zscan(const std::string &name, const std::string &key_start,
	int64_t *score_start, int64_t *score_end,
	uint64_t limit, std::vector<std::string> *ret)
{
	std::string s_score_start = score_start? int_to_str(*score_start) : "";
	std::string s_score_end = score_end? int_to_str(*score_end) : "";
	std::string s_limit = uint64_to_str(limit);
	const std::vector<std::string> *resp;
	resp = this->request("zscan", name, key_start, s_score_start, s_score_end, s_limit);
	return _read_list(resp, ret);
}

Status ClientImpl::zrscan(const std::string &name, const std::string &key_start,
	int64_t *score_start, int64_t *score_end,
	uint64_t limit, std::vector<std::string> *ret)
{
	std::string s_score_start = score_start? int_to_str(*score_start) : "";
	std::string s_score_end = score_end? int_to_str(*score_end) : "";
	std::string s_limit = uint64_to_str(limit);
	const std::vector<std::string> *resp;
	resp = this->request("zrscan", name, key_start, s_score_start, s_score_end, s_limit);
	return _read_list(resp, ret);
}

}; // namespace ssdb
