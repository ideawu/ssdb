#ifndef SSDB_SERVER_H_
#define SSDB_SERVER_H_

#include "include.h"
#include <map>
#include <vector>
#include <string>
#include "util/thread.h"
#include "ssdb.h"
#include "backend_dump.h"
#include "backend_sync.h"

#define PROC_OK			0
#define PROC_ERROR		-1
#define PROC_BACKEND	100

typedef std::vector<Bytes> Request;
typedef std::vector<std::string> Response;


class Server;
typedef int (*proc_t)(Server *serv, Link *link, const Request &req, Response *resp);

typedef struct{
	static const int FLAG_READ		= (1 << 0);
	static const int FLAG_WRITE		= (1 << 1);
	static const int FLAG_BACKEND	= (1 << 1);

	const char *name;
	const char *sflags;
	int flags;
	proc_t proc;
	uint64_t calls;
	double time_wait;
	double time_proc;
}Command;

typedef struct _ProcJob{
	int result;
	Server *serv;
	Link *link;
	Command *cmd;
	double stime;
	double time_wait;
	double time_proc;
	
	_ProcJob(){
		result = 0;
		serv = NULL;
		link = NULL;
		cmd = NULL;
		stime = 0;
		time_wait = 0;
		time_proc = 0;
	}
}ProcJob; // Request


class Server{
	private:
		static const int MAX_WRITERS = 1;
	public:
		SSDB *ssdb;
		BackendDump *backend_dump;
		BackendSync *backend_sync;

		Server(SSDB *ssdb);
		~Server();
		void proc(ProcJob *job);

		// WARN: pipe latency is about 20 us, it is really slow!
		// until I found a really fast selectable queue, this feature won't be enabled.
		/*
		class WriteProc : public WorkerPool<WriteProc, ProcJob>::Worker{
		public:
			void init();
			void destroy();
			int proc(ProcJob *job);
		};
		WorkerPool<WriteProc, ProcJob> writer;
		*/
};

template<class T>
static std::string serialize_req(T &req){
	std::string ret;
	char buf[50];
	for(int i=0; i<req.size(); i++){
		if(i >= 5 && i < req.size() - 1){
			sprintf(buf, "[%d more...]", (int)req.size() - i);
			ret.append(buf);
			break;
		}
		if(((req[0] == "get" || req[0] == "set") && i == 1) || req[i].size() < 30){
			std::string h = hexmem(req[i].data(), req[i].size());
			ret.append(h);
		}else{
			sprintf(buf, "[%d]", (int)req[i].size());
			ret.append(buf);
		}
		if(i < req.size() - 1){
			ret.append(" ");
		}
	}
	return ret;
}

#endif
