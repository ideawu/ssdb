#ifndef SSDB_API_CPP
#define SSDB_API_CPP

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>
#include <string>
#include <vector>
#include <map>

namespace ssdb{

/**
 * Client requests' return status.
 */
class Status{
public:
	/**
	 * Returns <code>true</code> if the request succeeded.
	 */
	bool ok(){
		return code_ == "ok";
	}
	/**
	 * Returns <code>true</code> if the requested key is not found. When this method
	 * returns <code>true</code>, ok() will always returns <code>false</code>.
	 */
	bool not_found(){
		return code_ == "not_found";
	}
	/**
	 * Returns <code>true</code> if error occurs during the request.
	 * It might be a server error, or a client error.
	 */
	bool error(){
		return code_ != "ok";
	}
	/**
	 * The response code.
	 */
	std::string code(){
		return code_;
	}

	Status(){}
	Status(const std::string &code){
		code_ = code;
	}
	Status(const std::vector<std::string> *resp){
		if(resp && resp->size() > 0){
			code_ = resp->at(0);
		}else{
			code_ = "error";
		}
	}
private:
	std::string code_;
};

/**
 * The SSDB client used to connect to SSDB server.
 */
class Client{
public:
	static Client* connect(const char *ip, int port);
	static Client* connect(const std::string &ip, int port);
	Client(){};
	virtual ~Client(){};

	/// @name Free hand methods
	/// All these methods return NULL if error; vector<std::string> if response ready,
	/// the first element is response code.
	/// @{
	virtual const std::vector<std::string>* request(const std::vector<std::string> &req) = 0;
	virtual const std::vector<std::string>* request(const std::string &cmd) = 0;
	virtual const std::vector<std::string>* request(const std::string &cmd, const std::string &s2) = 0;
	virtual const std::vector<std::string>* request(const std::string &cmd, const std::string &s2, const std::string &s3) = 0;
	virtual const std::vector<std::string>* request(const std::string &cmd, const std::string &s2, const std::string &s3, const std::string &s4) = 0;
	virtual const std::vector<std::string>* request(const std::string &cmd, const std::string &s2, const std::string &s3, const std::string &s4, const std::string &s5) = 0;
	virtual const std::vector<std::string>* request(const std::string &cmd, const std::string &s2, const std::string &s3, const std::string &s4, const std::string &s5, const std::string &s6) = 0;
	virtual const std::vector<std::string>* request(const std::string &cmd, const std::vector<std::string> &s2) = 0;
	virtual const std::vector<std::string>* request(const std::string &cmd, const std::string &s2, const std::vector<std::string> &s3) = 0;
	/// @}
	
	virtual Status dbsize(int64_t *ret) = 0;
	virtual Status get_kv_range(std::string *start, std::string *end) = 0;
	virtual Status set_kv_range(const std::string &start, const std::string &end) = 0;

	/// @name KV methods
	/// @{
	virtual Status get(const std::string &key, std::string *val) = 0;
	virtual Status set(const std::string &key, const std::string &val) = 0;
	/**
	 * Set the value of the key, with a time to live.
	 */
	virtual Status setx(const std::string &key, const std::string &val, int ttl) = 0;
	virtual Status del(const std::string &key) = 0;
	virtual Status incr(const std::string &key, int64_t incrby, int64_t *ret) = 0;
	/**
	 * @param key_start Empty string means no limit.
	 * @param key_end Empty string means no limit.
	 */
	virtual Status keys(const std::string &key_start, const std::string &key_end,
		uint64_t limit, std::vector<std::string> *ret) = 0;
	/**
	 * Return key-value pairs.
	 * The two elements at ret[n] and ret[n+1] form a key-value pair, n=0,2,4,...
	 */
	virtual Status scan(const std::string &key_start, const std::string &key_end,
	 	uint64_t limit, std::vector<std::string> *ret) = 0;
	/**
	 * Return key-value pairs.
	 * The two elements at ret[n] and ret[n+1] form a key-value pair, n=0,2,4,...
	 */
	virtual Status rscan(const std::string &key_start, const std::string &key_end,
		uint64_t limit, std::vector<std::string> *ret) = 0;
	/**
	 * Return key-value pairs.
	 * The two elements at ret[n] and ret[n+1] form a key-value pair, n=0,2,4,...
	 */
	virtual Status multi_get(const std::vector<std::string> &keys, std::vector<std::string> *vals) = 0;
	virtual Status multi_set(const std::map<std::string, std::string> &kvs) = 0;
	virtual Status multi_del(const std::vector<std::string> &keys) = 0;
	/// @}


	/// @name Map(Hash) methods
	/// @{
	virtual Status hget(const std::string &name, const std::string &key, std::string *val) = 0;
	virtual Status hset(const std::string &name, const std::string &key, const std::string &val) = 0;
	virtual Status hdel(const std::string &name, const std::string &key) = 0;
	virtual Status hincr(const std::string &name, const std::string &key, int64_t incrby, int64_t *ret) = 0;
	virtual Status hsize(const std::string &name, int64_t *ret) = 0;
	/**
	 * Delete all of the keys in a hashmap, return the number of keys deleted.
	 */
	virtual Status hclear(const std::string &name, int64_t *ret=NULL) = 0;
	/**
	 * @param key_start Empty string means no limit.
	 * @param key_end Empty string means no limit.
	 */
	virtual Status hkeys(const std::string &name, 
		const std::string &key_start, const std::string &key_end,
		uint64_t limit, std::vector<std::string> *ret) = 0;
	/**
	 * Return key-value pairs.
	 * The two elements at ret[n] and ret[n+1] form a key-value pair, n=0,2,4,...
	 */
	virtual Status hscan(const std::string &name, 
		const std::string &key_start, const std::string &key_end,
	 	uint64_t limit, std::vector<std::string> *ret) = 0;
	/**
	 * Return key-value pairs.
	 * The two elements at ret[n] and ret[n+1] form a key-value pair, n=0,2,4,...
	 */
	virtual Status hrscan(const std::string &name, 
		const std::string &key_start, const std::string &key_end,
		uint64_t limit, std::vector<std::string> *ret) = 0;
	/**
	 * Return key-value pairs.
	 * The two elements at ret[n] and ret[n+1] form a key-value pair, n=0,2,4,...
	 */
	virtual Status multi_hget(const std::string &name, const std::vector<std::string> &keys,
		std::vector<std::string> *ret) = 0;
	virtual Status multi_hset(const std::string &name, const std::map<std::string, std::string> &kvs) = 0;
	virtual Status multi_hdel(const std::string &name, const std::vector<std::string> &keys) = 0;
	/// @}


	/// @name Zset methods
	/// @{
	virtual Status zget(const std::string &name, const std::string &key, int64_t *ret) = 0;
	virtual Status zset(const std::string &name, const std::string &key, int64_t score) = 0;
	virtual Status zdel(const std::string &name, const std::string &key) = 0;
	virtual Status zincr(const std::string &name, const std::string &key, int64_t incrby, int64_t *ret) = 0;
	virtual Status zsize(const std::string &name, int64_t *ret) = 0;
	/**
	 * Delete all of the keys in a zset, return the number of keys deleted.
	 */
	virtual Status zclear(const std::string &name, int64_t *ret=NULL) = 0;
	/**
	 * <b>Important! This method may be extremly SLOW!</b>
	 */
	virtual Status zrank(const std::string &name, const std::string &key, int64_t *ret) = 0;
	/**
	 * <b>Important! This method may be extremly SLOW!</b>
	 */
	virtual Status zrrank(const std::string &name, const std::string &key, int64_t *ret) = 0;
	/**
	 * <b>Important! This method is SLOW for large offset!</b>
	 */
	virtual Status zrange(const std::string &name,
		uint64_t offset, uint64_t limit,
		std::vector<std::string> *ret) = 0;
	/**
	 * <b>Important! This method is SLOW for large offset!</b>
	 */
	virtual Status zrrange(const std::string &name,
		uint64_t offset, uint64_t limit,
		std::vector<std::string> *ret) = 0;
	/**
	 * @param score_start NULL means no limit.
	 * @param score_end NULL means no limit.
	 */
	virtual Status zkeys(const std::string &name, const std::string &key_start,
		int64_t *score_start, int64_t *score_end,
		uint64_t limit, std::vector<std::string> *ret) = 0;
	/**
	 * Return key-score pairs.
	 * The two elements at ret[n] and ret[n+1] form a key-score pair, n=0,2,4,...
	 */
	virtual Status zscan(const std::string &name, const std::string &key_start,
		int64_t *score_start, int64_t *score_end,
		uint64_t limit, std::vector<std::string> *ret) = 0;
	/**
	 * Return key-score pairs.
	 * The two elements at ret[n] and ret[n+1] form a key-score pair, n=0,2,4,...
	 */
	virtual Status zrscan(const std::string &name, const std::string &key_start,
		int64_t *score_start, int64_t *score_end,
		uint64_t limit, std::vector<std::string> *ret) = 0;
	/**
	 * Return key-value pairs.
	 * The two elements at ret[n] and ret[n+1] form a key-value pair, n=0,2,4,...
	 */
	virtual Status multi_zget(const std::string &name, const std::vector<std::string> &keys,
		std::vector<std::string> *scores) = 0;
	virtual Status multi_zset(const std::string &name, const std::map<std::string, int64_t> &kss) = 0;
	virtual Status multi_zdel(const std::string &name, const std::vector<std::string> &keys) = 0;
	/// @}

	virtual Status qpush(const std::string &name, const std::string &item, int64_t *ret_size=NULL) = 0;
	virtual Status qpush(const std::string &name, const std::vector<std::string> &items, int64_t *ret_size=NULL) = 0;
	virtual Status qpop(const std::string &name, std::string *ret) = 0;
	virtual Status qpop(const std::string &name, int64_t limit, std::vector<std::string> *ret) = 0;
	virtual Status qslice(const std::string &name, int64_t begin, int64_t end, std::vector<std::string> *ret) = 0;
	virtual Status qrange(const std::string &name, int64_t begin, int64_t limit, std::vector<std::string> *ret) = 0;
	virtual Status qclear(const std::string &name, int64_t *ret=NULL) = 0;
private:
	// No copying allowed
	Client(const Client&);
	void operator=(const Client&);
};

}; // namespace ssdb

#endif
