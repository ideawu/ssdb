#include "server.h"
#include "../util/config.h"

DEF_PROC(hello);

int main(int argc, char **argv){
	Config conf;
	conf.set("server.port", "9000");
	NetworkServer serv;
	serv.init(conf);
	// register command procedure
	serv.proc_map.set_proc("hello", proc_hello);
	serv.serve();
	return 0;
}

int proc_hello(NetworkServer *net, Link *link, const Request &req, Response *resp){
	resp->push_back("ok");
	resp->push_back("world!");
	if(req.size() > 1){
		// The first argument start at index 1, just like argv.
		resp->push_back(req[1].String());
	}
	return 0;
}
