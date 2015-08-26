#ifndef SSDB_API_IMPL_CPP
#define SSDB_API_IMPL_CPP

#include "SSDB_client.h"
#include "net/link.h"

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
	virtual const std::vector<std::string>* request(const std::string &cmd, const std::string &s2, const std::string &s3, const std::string &s4, const std::string &s5, const std::string &s6);
	virtual const std::vector<std::string>* request(const std::string &cmd, const std::vector<std::string> &s2);
	virtual const std::vector<std::string>* request(const std::string &cmd, const std::string &s2, const std::vector<std::string> &s3);

	virtual Status dbsize(int64_t *ret);
	virtual Status get_kv_range(std::string *start, std::string *end);
	virtual Status set_kv_range(const std::string &start, const std::string &end);

	virtual Status get(const std::string &key, std::string *val);
	virtual Status set(const std::string &key, const std::string &val);
	virtual Status setx(const std::string &key, const std::string &val, int ttl);
	virtual Status del(const std::string &key);
	virtual Status incr(const std::string &key, int64_t incrby, int64_t *ret);
	virtual Status keys(const std::string &key_start, const std::string &key_end,
		uint64_t limit, std::vector<std::string> *ret);
	virtual Status scan(const std::string &key_start, const std::string &key_end,
		uint64_t limit, std::vector<std::string> *ret);
	virtual Status rscan(const std::string &key_start, const std::string &key_end,
		uint64_t limit, std::vector<std::string> *ret);
	virtual Status multi_get(const std::vector<std::string> &keys, std::vector<std::string> *ret);
	virtual Status multi_set(const std::map<std::string, std::string> &kvs);
	virtual Status multi_del(const std::vector<std::string> &keys);
	
	virtual Status hget(const std::string &name, const std::string &key, std::string *val);
	virtual Status hset(const std::string &name, const std::string &key, const std::string &val);
	virtual Status hdel(const std::string &name, const std::string &key);
	virtual Status hincr(const std::string &name, const std::string &key, int64_t incrby, int64_t *ret);
	virtual Status hsize(const std::string &name, int64_t *ret);
	virtual Status hclear(const std::string &name, int64_t *ret=NULL);
	virtual Status hkeys(const std::string &name, const std::string &key_start, const std::string &key_end,
		uint64_t limit, std::vector<std::string> *ret);
	virtual Status hscan(const std::string &name, const std::string &key_start, const std::string &key_end,
		uint64_t limit, std::vector<std::string> *ret);
	virtual Status hrscan(const std::string &name, const std::string &key_start, const std::string &key_end,
		uint64_t limit, std::vector<std::string> *ret);
	virtual Status multi_hget(const std::string &name, const std::vector<std::string> &keys,
		std::vector<std::string> *ret);
	virtual Status multi_hset(const std::string &name, const std::map<std::string, std::string> &kvs);
	virtual Status multi_hdel(const std::string &name, const std::vector<std::string> &keys);

	virtual Status zget(const std::string &name, const std::string &key, int64_t *ret);
	virtual Status zset(const std::string &name, const std::string &key, int64_t score);
	virtual Status zdel(const std::string &name, const std::string &key);
	virtual Status zincr(const std::string &name, const std::string &key, int64_t incrby, int64_t *ret);
	virtual Status zsize(const std::string &name, int64_t *ret);
	virtual Status zclear(const std::string &name, int64_t *ret=NULL);
	virtual Status zrank(const std::string &name, const std::string &key, int64_t *ret);
	virtual Status zrrank(const std::string &name, const std::string &key, int64_t *ret);
	virtual Status zrange(const std::string &name,
		uint64_t offset, uint64_t limit,
		std::vector<std::string> *ret);
	virtual Status zrrange(const std::string &name,
		uint64_t offset, uint64_t limit,
		std::vector<std::string> *ret);
	virtual Status zkeys(const std::string &name, const std::string &key_start,
		int64_t *score_start, int64_t *score_end,
		uint64_t limit, std::vector<std::string> *ret);
	virtual Status zscan(const std::string &name, const std::string &key_start,
		int64_t *score_start, int64_t *score_end,
		uint64_t limit, std::vector<std::string> *ret);
	virtual Status zrscan(const std::string &name, const std::string &key_start,
		int64_t *score_start, int64_t *score_end,
		uint64_t limit, std::vector<std::string> *ret);
	virtual Status multi_zget(const std::string &name, const std::vector<std::string> &keys,
		std::vector<std::string> *scores);
	virtual Status multi_zset(const std::string &name, const std::map<std::string, int64_t> &kss);
	virtual Status multi_zdel(const std::string &name, const std::vector<std::string> &keys);

	virtual Status qpush(const std::string &name, const std::string &item, int64_t *ret_size=NULL);
	virtual Status qpush(const std::string &name, const std::vector<std::string> &items, int64_t *ret_size=NULL);
	virtual Status qpop(const std::string &name, std::string *item);
	virtual Status qpop(const std::string &name, int64_t limit, std::vector<std::string> *ret);
	virtual Status qslice(const std::string &name, int64_t begin, int64_t end, std::vector<std::string> *ret);
	virtual Status qrange(const std::string &name, int64_t begin, int64_t limit, std::vector<std::string> *ret);
	virtual Status qclear(const std::string &name, int64_t *ret=NULL);
};

}; // namespace ssdb

#endif
