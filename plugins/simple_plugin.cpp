
#include "plugin.h"

static int proc_hi(Server *serv, Link *link, const Request &req, Response *resp);
static int proc_echo(Server *serv, Link *link, const Request &req, Response *resp);

static Command cmd_hi = {"hi", "r", 0, proc_hi, 0, 0, 0};
static Command cmd_echo = {"echo", "r", 0, proc_echo, 0, 0, 0};

extern  "C" {

int ssdb_plugin_init(void *_serv, const char* opt) {
	//log_info("Hi, i am ssdb plugin");
	//ssdb_add_command(NULL);
	Server *serv = (Server*)_serv;
	log_info("add custom command hi");
	serv->add_command(&cmd_hi);
	serv->add_command(&cmd_echo);
	return 0;
}
}

static int proc_hi(Server *serv, Link *link, const Request &req, Response *resp){
	resp->push_back("ok");
	return 0;
}

static int proc_echo(Server *serv, Link *link, const Request &req, Response *resp) {
	if (req.size() != 2) {
		resp->push_back("give me a value");
	} else {
		resp->push_back(req[1] == "god" ? "God is comming" : "sorry");
	}
	return 0;
}
