#ifndef SSDB_PROC_H_
#define SSDB_PROC_H_

#include <map>
#include <vector>
#include <string>

#define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
#if GCC_VERSION >= 403
	#include <tr1/unordered_map>
#else
	#include <ext/hash_map>
#endif

#include "ssdb.h"
#include "backend_dump.h"
#include "backend_sync.h"

#define PROC_OK			0
#define PROC_ERROR		-1
#define PROC_BACKEND	100

typedef std::vector<Bytes> Request;
typedef std::vector<std::string> Response;

class CommandProc{
	public:
		CommandProc(const SSDB *ssdb);
		~CommandProc();
		int proc(const Link &link, const Request &req, Response *resp);

	private:
		struct BytesEqual{
			bool operator()(const Bytes &s1, const Bytes &s2) const {
				return (bool)(s1.compare(s2) == 0);
			}
		};
		struct BytesHash{
			size_t operator()(const Bytes &s1) const {
				unsigned long __h = 0;
				const char *p = s1.data();
				for (int i=0 ; i<s1.size(); i++)
					__h = 5*__h + p[i];
				return size_t(__h);
			}
		};

		typedef int (CommandProc::*proc_t)(const Link &link, const Request &req, Response *resp);
#if GCC_VERSION >= 403
		typedef std::tr1::unordered_map<Bytes, proc_t, BytesHash, BytesEqual> proc_map_t;
#else
		typedef __gnu_cxx::hash_map<Bytes, proc_t, BytesHash, BytesEqual> proc_map_t;
#endif
	private:
		const SSDB *ssdb;
		proc_map_t proc_map;
		BackendDump *backend_dump;
		BackendSync *backend_sync;

		int proc_set(const Link &link, const Request &req, Response *resp);
		int proc_get(const Link &link, const Request &req, Response *resp);
		int proc_del(const Link &link, const Request &req, Response *resp);
		int proc_scan(const Link &link, const Request &req, Response *resp);
		int proc_keys(const Link &link, const Request &req, Response *resp);
		int proc_incr(const Link &link, const Request &req, Response *resp);
		int proc_decr(const Link &link, const Request &req, Response *resp);
		int proc_multi_set(const Link &link, const Request &req, Response *resp);
		int proc_multi_get(const Link &link, const Request &req, Response *resp);
		int proc_multi_del(const Link &link, const Request &req, Response *resp);

		int proc_zset(const Link &link, const Request &req, Response *resp);
		int proc_zget(const Link &link, const Request &req, Response *resp);
		int proc_zdel(const Link &link, const Request &req, Response *resp);
		int proc_zscan(const Link &link, const Request &req, Response *resp);
		int proc_zkeys(const Link &link, const Request &req, Response *resp);
		int proc_zincr(const Link &link, const Request &req, Response *resp);
		int proc_zdecr(const Link &link, const Request &req, Response *resp);

		int proc_hset(const Link &link, const Request &req, Response *resp);
		int proc_hget(const Link &link, const Request &req, Response *resp);
		int proc_hdel(const Link &link, const Request &req, Response *resp);
		int proc_hscan(const Link &link, const Request &req, Response *resp);
		int proc_hkeys(const Link &link, const Request &req, Response *resp);
		int proc_hincr(const Link &link, const Request &req, Response *resp);
		int proc_hdecr(const Link &link, const Request &req, Response *resp);

		int proc_dump(const Link &link, const Request &req, Response *resp);
		int proc_sync(const Link &link, const Request &req, Response *resp);
};
#endif
