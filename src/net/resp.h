#ifndef NET_RESP_H_
#define NET_RESP_H_

#include <string>
#include <vector>

class Response
{
public:
	std::vector<std::string> resp;

	int size() const;
	void push_back(const std::string &s);
	void add(int s);
	void add(int64_t s);
	void add(double s);

	void reply_status(int status, const char *errmsg=NULL);
	void reply_bool(int status, const char *errmsg=NULL);
	void reply_int(int status, int64_t val);
	// the same as Redis.REPLY_BULK
	void reply_get(int status, const std::string *val=NULL, const char *errmsg=NULL);
	void reply_list(int status, const std::vector<std::string> &list);
};

#endif
