#ifndef SSDB_API_CPP
#define SSDB_API_CPP

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>
#include <string>
#include <vector>

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
	Status(const std::vector<std::string> *resp){
		if(resp->size() > 0){
			code_ = resp->at(0);
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
	/// @}

	/// @name Semantic methods
	/// Not implemented!!!
	/// @{
	virtual Status get(const std::string &key, std::string *val) = 0;
	virtual Status set(const std::string &key, const std::string &val) = 0;
	virtual Status del(const std::string &key) = 0;
	virtual Status incr(const std::string &key, int64_t incrby=1, int64_t *ret=NULL) = 0;
	/**
	 * @param key_start Empty string means no limit.
	 * @param key_end Empty string means no limit.
	 */
	virtual Status keys(const std::string &key_start, const std::string &key_end,
		int limit, std::vector<std::string> *ret) = 0;
	/**
	 * Return key-value pairs.
	 * The two elements at ret[n] and ret[n+1] form a key-value pair, n=0,2,4,...
	 */
	virtual Status scan(const std::string &key_start, const std::string &key_end,
	 	int limit, std::vector<std::string> *ret) = 0;
	/**
	 * Return key-value pairs.
	 * The two elements at ret[n] and ret[n+1] form a key-value pair, n=0,2,4,...
	 */
	virtual Status rscan(const std::string &key_start, const std::string &key_end,
		int limit, std::vector<std::string> *ret) = 0;
	/// @}
private:
	// No copying allowed
	Client(const Client&);
	void operator=(const Client&);
};

}; // namespace ssdb

#endif
