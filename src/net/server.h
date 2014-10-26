#ifndef NET_SERVER_H_
#define NET_SERVER_H_

#include <inttypes.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <assert.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <vector>

#include "../util/fde.h"
#include "resp.h"
#include "proc.h"
#include "worker.h"

class Link;
class Config;
class IpFilter;
class Fdevents;

typedef std::vector<Link *> ready_list_t;

class Server
{
private:
	int tick_interval;
	int status_report_ticks;
	
	std::string pidfile;

	Config *conf;
	Link *serv_link;
	IpFilter *ip_filter;
	Fdevents *fdes;

	void check_pidfile();
	void write_pidfile();
	void remove_pidfile();

	Link* accept_link();
	int proc_result(ProcJob *job, ready_list_t *ready_list);
	int proc_client_event(const Fdevent *fde, ready_list_t *ready_list);

	void proc(ProcJob *job);

	static const int READER_THREADS = 10;
	static const int WRITER_THREADS = 1;
	ProcWorkerPool *writer;
	ProcWorkerPool *reader;

	void init(const char *conf_file, bool is_daemon);

protected:
	void usage(int argc, char **argv);

public:
	ProcMap proc_map;
	int link_count;
	bool need_auth;
	std::string password;

	Server();
	~Server();
	
	void run(const char *conf_file, bool is_daemon);
};


#endif
