#ifndef SSDB_PROC_H_
#define SSDB_PROC_H_

#include "include.h"
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
		CommandProc(SSDB *ssdb);
		~CommandProc();
		int proc(const Link &link, const Request &req, Response *resp);

		typedef int (CommandProc::*proc_t)(const Link &link, const Request &req, Response *resp);
		
		typedef struct{
			const char *name;
			const char *sflags;
			int flags;
			proc_t proc;
			uint64_t calls;
			double ts;
		}Command;
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
		
		static const int FLAG_READ		= (1 << 0);
		static const int FLAG_WRITE		= (1 << 1);
		static const int FLAG_BACKEND	= (1 << 1);
		
#if GCC_VERSION >= 403
		typedef std::tr1::unordered_map<Bytes, Command *, BytesHash, BytesEqual> proc_map_t;
#else
		typedef __gnu_cxx::hash_map<Bytes, Command *, BytesHash, BytesEqual> proc_map_t;
#endif

		SSDB *ssdb;
		proc_map_t proc_map;
		BackendDump *backend_dump;
		BackendSync *backend_sync;

	public:
#define DEF_PROC(f) int proc_##f(const Link &link, const Request &req, Response *resp)
		DEF_PROC(info);
		
		DEF_PROC(get);
		DEF_PROC(set);
		DEF_PROC(del);
		DEF_PROC(incr);
		DEF_PROC(decr);
		DEF_PROC(scan);
		DEF_PROC(rscan);
		DEF_PROC(keys);
		DEF_PROC(exists);
		DEF_PROC(multi_exists);
		DEF_PROC(multi_get);
		DEF_PROC(multi_set);
		DEF_PROC(multi_del);

		DEF_PROC(hsize);
		DEF_PROC(hget);
		DEF_PROC(hset);
		DEF_PROC(hdel);
		DEF_PROC(hincr);
		DEF_PROC(hdecr);
		DEF_PROC(hscan);
		DEF_PROC(hrscan);
		DEF_PROC(hkeys);
		DEF_PROC(hlist);
		DEF_PROC(hexists);
		DEF_PROC(multi_hexists);
		DEF_PROC(multi_hsize);
		DEF_PROC(multi_hget);
		DEF_PROC(multi_hset);
		DEF_PROC(multi_hdel);

		DEF_PROC(zsize);
		DEF_PROC(zget);
		DEF_PROC(zset);
		DEF_PROC(zdel);
		DEF_PROC(zincr);
		DEF_PROC(zdecr);
		DEF_PROC(zscan);
		DEF_PROC(zrscan);
		DEF_PROC(zkeys);
		DEF_PROC(zlist);
		DEF_PROC(zexists);
		DEF_PROC(multi_zexists);
		DEF_PROC(multi_zsize);
		DEF_PROC(multi_zget);
		DEF_PROC(multi_zset);
		DEF_PROC(multi_zdel);

		DEF_PROC(dump);
		DEF_PROC(sync);
#undef DEF_PROC
};
#endif
