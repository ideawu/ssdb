#ifndef SSDB_API_IMPL_CPP
#define SSDB_API_IMPL_CPP

#include "SSDB.h"
#include "link.h"

namespace ssdb{

class ClientImpl : public Client{
private:
	friend class Client;
	
	Link *link;
	std::vector<std::string> resp_;
public:
	ClientImpl();
	~ClientImpl();

	virtual const std::vector<std::string>* request(const std::vector<std::string> &req);
	virtual const std::vector<std::string>* request(const std::string &cmd);
	virtual const std::vector<std::string>* request(const std::string &cmd, const std::string &s2);
	virtual const std::vector<std::string>* request(const std::string &cmd, const std::string &s2, const std::string &s3);
	virtual const std::vector<std::string>* request(const std::string &cmd, const std::string &s2, const std::string &s3, const std::string &s4);
	virtual const std::vector<std::string>* request(const std::string &cmd, const std::string &s2, const std::string &s3, const std::string &s4, const std::string &s5);
	
	virtual Status get(const std::string &key, std::string *val);
	virtual Status set(const std::string &key, const std::string &val);
	virtual Status del(const std::string &key);
	virtual Status incr(const std::string &key, int64_t incrby=1, int64_t *ret=NULL);
	virtual Status keys(const std::string &key_start, const std::string &key_end,
		int limit, std::vector<std::string> *ret);
	virtual Status scan(const std::string &key_start, const std::string &key_end,
		int limit, std::vector<std::string> *ret);
	virtual Status rscan(const std::string &key_start, const std::string &key_end,
		int limit, std::vector<std::string> *ret);
};

}; // namespace ssdb

#endif
