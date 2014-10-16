#include "include.h"
#include <vector>
#include <string>
#include <list>
#include "version.h"
#include "ssdb.h"
#include "link.h"
#include "serv.h"
#include "util/fde.h"
#include "util/config.h"
#include "util/daemon.h"
#include "util/strings.h"
#include "util/file.h"
#include "util/ip_filter.h"
#include <dlfcn.h>

#define TICK_INTERVAL       100 // ms
#define STATUS_REPORT_TICKS (300 * 1000/TICK_INTERVAL) // second

void welcome();
void usage(int argc, char **argv);
void signal_handler(int sig);
void init(int argc, char **argv);
void run(int argc, char **argv);
void write_pidfile();
void check_pidfile();
void check_pidfile();
void remove_pidfile();

Config *conf = NULL;
SSDB *ssdb = NULL;
Link *serv_link = NULL;
IpFilter *ip_filter = NULL;
Fdevents *fdes = NULL;
Server *serv = NULL;

typedef std::vector<Link *> ready_list_t;

volatile bool quit = false;
volatile uint32_t g_ticks = 0;

int main(int argc, char **argv){
	welcome();
	init(argc, argv);
	
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
#ifndef __CYGWIN__
	signal(SIGALRM, signal_handler);
	{
		struct itimerval tv;
		tv.it_interval.tv_sec = (TICK_INTERVAL / 1000);
		tv.it_interval.tv_usec = (TICK_INTERVAL % 1000) * 1000;
		tv.it_value.tv_sec = 1;
		tv.it_value.tv_usec = 0;
		setitimer(ITIMER_REAL, &tv, NULL);
	}
#endif
	
	fdes = new Fdevents();
	run(argc, argv);
	remove_pidfile();

	if(serv_link){
		log_info("shutdown network");
		delete serv_link;
	}
	if(ssdb){
		log_debug("free ssdb");
		delete ssdb;
	}
	if(conf){
		log_debug("free conf");
		delete conf;
	}
	if(fdes){
		delete fdes;
	}
	log_info("ssdb server exit.");
	return 0;
}

Link* accept_link(){
	Link *link = serv_link->accept();
	if(link == NULL){
		log_error("accept failed! %s", strerror(errno));
		return NULL;
	}
	if(!ip_filter->check_pass(link->remote_ip)){
		log_debug("ip_filter deny link from %s:%d", link->remote_ip, link->remote_port);
		delete link;
		return NULL;
	}
				
	link->nodelay();
	link->noblock();
	link->create_time = millitime();
	link->active_time = link->create_time;
	return link;
}

int proc_result(ProcJob &job, ready_list_t &ready_list){	
	Link *link = job.link;
	int len;
			
	if(job.cmd){
		job.cmd->calls += 1;
		job.cmd->time_wait += job.time_wait;
		job.cmd->time_proc += job.time_proc;
	}
	if(job.result == PROC_ERROR){
		log_info("fd: %d, proc error, delete link", link->fd());
		goto proc_err;
	}
	
	len = link->write();
	//log_debug("write: %d", len);
	if(len < 0){
		log_debug("fd: %d, write: %d, delete link", link->fd(), len);
		goto proc_err;
	}

	if(!link->output->empty()){
		fdes->set(link->fd(), FDEVENT_OUT, 1, link);
	}
	if(link->input->empty()){
		fdes->set(link->fd(), FDEVENT_IN, 1, link);
	}else{
		fdes->clr(link->fd(), FDEVENT_IN);
		ready_list.push_back(link);
	}
	return PROC_OK;

proc_err:
	fdes->del(link->fd());
	delete link;
	return PROC_ERROR;
}

/*
event:
	read => ready_list OR close
	write => NONE
proc =>
	done: write & (read OR ready_list)
	async: stop (read & write)
	
1. When writing to a link, it may happen to be in the ready_list,
so we cannot close that link in write process, we could only
just mark it as closed.

2. When reading from a link, it is never in the ready_list, so it
is safe to close it in read process, also safe to put it into
ready_list.

3. Ignore FDEVENT_ERR

A link is in either one of these places:
	1. ready list
	2. async worker queue
So it safe to delete link when processing ready list and async worker result.
*/
int proc_client_event(const Fdevent *fde, ready_list_t &ready_list){	
	Link *link = (Link *)fde->data.ptr;
	if(fde->events & FDEVENT_IN){
		ready_list.push_back(link);
		if(link->error()){
			return 0;
		}
		int len = link->read();
		//log_debug("fd: %d read: %d", link->fd(), len);
		if(len <= 0){
			log_debug("fd: %d, read: %d, delete link", link->fd(), len);
			link->mark_error();
			return 0;
		}
	}
	if(fde->events & FDEVENT_OUT){
		if(link->error()){
			return 0;
		}
		int len = link->write();
		if(len <= 0){
			log_debug("fd: %d, write: %d, delete link", link->fd(), len);
			link->mark_error();
			return 0;
		}
		if(link->output->empty()){
			fdes->clr(link->fd(), FDEVENT_OUT);
		}
	}
	return 0;
}

void run(int argc, char **argv){
	ready_list_t ready_list;
	ready_list_t ready_list_2;
	ready_list_t::iterator it;
	const Fdevents::events_t *events;

	fdes->set(serv_link->fd(), FDEVENT_IN, 0, serv_link);
	fdes->set(serv->reader->fd(), FDEVENT_IN, 0, serv->reader);
	fdes->set(serv->writer->fd(), FDEVENT_IN, 0, serv->writer);
	
	uint32_t last_ticks = g_ticks;
	
	while(!quit){
		// status report
		if((uint32_t)(g_ticks - last_ticks) >= STATUS_REPORT_TICKS){
			last_ticks = g_ticks;
			log_info("ssdb working, links: %d", serv->link_count);
		}
		
		ready_list.swap(ready_list_2);
		ready_list_2.clear();
		
		if(!ready_list.empty()){
			// ready_list not empty, so we should return immediately
			events = fdes->wait(0);
		}else{
			events = fdes->wait(50);
		}
		if(events == NULL){
			log_fatal("events.wait error: %s", strerror(errno));
			break;
		}
		
		for(int i=0; i<(int)events->size(); i++){
			const Fdevent *fde = events->at(i);
			if(fde->data.ptr == serv_link){
				Link *link = accept_link();
				if(link){
					serv->link_count ++;				
					log_debug("new link from %s:%d, fd: %d, links: %d",
						link->remote_ip, link->remote_port, link->fd(), serv->link_count);
					fdes->set(link->fd(), FDEVENT_IN, 1, link);
				}
			}else if(fde->data.ptr == serv->reader || fde->data.ptr == serv->writer){
				WorkerPool<Server::ProcWorker, ProcJob> *worker = (WorkerPool<Server::ProcWorker, ProcJob> *)fde->data.ptr;
				ProcJob job;
				if(worker->pop(&job) == 0){
					log_fatal("reading result from workers error!");
					exit(0);
				}
				if(proc_result(job, ready_list) == PROC_ERROR){
					serv->link_count --;
				}
			}else{
				proc_client_event(fde, ready_list);
			}
		}

		for(it = ready_list.begin(); it != ready_list.end(); it ++){
			Link *link = *it;
			if(link->error()){
				serv->link_count --;
				fdes->del(link->fd());
				delete link;
				continue;
			}

			const Request *req = link->recv();
			if(req == NULL){
				log_warn("fd: %d, link parse error, delete link", link->fd());
				serv->link_count --;
				fdes->del(link->fd());
				delete link;
				continue;
			}
			if(req->empty()){
				fdes->set(link->fd(), FDEVENT_IN, 1, link);
				continue;
			}
			
			link->active_time = millitime();

			ProcJob job;
			job.link = link;
			serv->proc(&job);
			if(job.result == PROC_THREAD){
				fdes->del(link->fd());
				continue;
			}
			if(job.result == PROC_BACKEND){
				fdes->del(link->fd());
				serv->link_count --;
				continue;
			}
			
			if(proc_result(job, ready_list_2) == PROC_ERROR){
				serv->link_count --;
			}
		} // end foreach ready link
	}
}


void welcome(){
	fprintf(stderr, "ssdb %s\n", SSDB_VERSION);
	fprintf(stderr, "Copyright (c) 2012-2014 ssdb.io\n");
	fprintf(stderr, "\n");
}

void usage(int argc, char **argv){
	printf("Usage:\n");
	printf("    %s [-d] /path/to/ssdb.conf\n", argv[0]);
	printf("Options:\n");
	printf("    -d    run as daemon\n");
}

void signal_handler(int sig){
	switch(sig){
		case SIGTERM:
		case SIGINT:{
			quit = true;
			break;
		}
		case SIGALRM:{
			g_ticks ++;
			break;
		}
	}
}

void init(int argc, char **argv){
	if(argc < 2){
		usage(argc, argv);
		exit(1);
	}

	bool is_daemon = false;
	const char *conf_file = NULL;
	for(int i=1; i<argc; i++){
		if(strcmp(argv[i], "-d") == 0){
			is_daemon = true;
		}else{
			conf_file = argv[i];
		}
	}

	if(conf_file == NULL){
		usage(argc, argv);
		exit(1);
	}

	if(!is_file(conf_file)){
		fprintf(stderr, "'%s' is not a file or not exists!\n", conf_file);
		exit(1);
	}

	conf = Config::load(conf_file);
	if(!conf){
		fprintf(stderr, "error loading conf file: '%s'\n", conf_file);
		exit(1);
	}
	{
		std::string conf_dir = real_dirname(conf_file);
		if(chdir(conf_dir.c_str()) == -1){
			fprintf(stderr, "error chdir: %s\n", conf_dir.c_str());
			exit(1);
		}
	}

	std::string work_dir;
	{
		work_dir = conf->get_str("work_dir");
		if(work_dir.empty()){
			work_dir = ".";
		}
		if(!is_dir(work_dir.c_str())){
			fprintf(stderr, "'%s' is not a directory or not exists!\n", work_dir.c_str());
			exit(1);
		}
		/*
		if(chdir(work_dir.c_str()) == -1){
			fprintf(stderr, "error chdir: %s\n", work_dir.c_str());
			exit(1);
		}
		*/
	}
	
	check_pidfile();

	std::string log_output;
	int log_rotate_size = 0;
	{ // logger
		int log_level = Logger::get_level(conf->get_str("logger.level"));
		log_rotate_size = conf->get_num("logger.rotate.size");
		log_output = conf->get_str("logger.output");
		if(log_output == ""){
			log_output = "stdout";
		}
		if(log_open(log_output.c_str(), log_level, true, log_rotate_size) == -1){
			fprintf(stderr, "error opening log file: %s\n", log_output.c_str());
			exit(1);
		}
	}

	log_info("ssdb-server %s", SSDB_VERSION);
	log_info("conf_file       : %s", conf_file);
	log_info("work_dir        : %s", work_dir.c_str());
	log_info("log_level       : %s", conf->get_str("logger.level"));
	log_info("log_output      : %s", log_output.c_str());
	log_info("log_rotate_size : %d", log_rotate_size);
	
	std::string password;
	ip_filter = new IpFilter();
	// init ip_filter
	{
		Config *cc = (Config *)conf->get("server");
		if(cc != NULL){
			std::vector<Config *> *children = &cc->children;
			std::vector<Config *>::iterator it;
			for(it = children->begin(); it != children->end(); it++){
				if((*it)->key == "allow"){
					const char *ip = (*it)->str();
					log_info("    allow %s", ip);
					ip_filter->add_allow(ip);
				}
				if((*it)->key == "deny"){
					const char *ip = (*it)->str();
					log_info("    deny %s", ip);
					ip_filter->add_deny(ip);
				}
			}
		}
	}
	
	{ // server
		const char *ip = conf->get_str("server.ip");
		int port = conf->get_num("server.port");
		
		serv_link = Link::listen(ip, port);
		if(serv_link == NULL){
			log_fatal("error opening server socket! %s", strerror(errno));
			fprintf(stderr, "error opening server socket! %s\n", strerror(errno));
			exit(1);
		}
		log_info("server listen on %s:%d", ip, port);

		password = conf->get_str("server.auth");
		if(password.size() && (password.size() < 32 || password == "very-strong-password")){
			log_fatal("weak password is not allowed!");
			fprintf(stderr, "WARNING! Weak password is not allowed!\n");
			exit(1);
		}
		if(password.empty()){
			log_info("server.auth off");
		}else{
			log_info("server.auth on");
		}
	}
	void *plugin_handle;
	{// plugin
		const char * plugin_path = conf->get_str("server.plugin_path");
		if (plugin_path) {
			log_info("load plugin %s", plugin_path);
			plugin_handle = dlopen(plugin_path, RTLD_LAZY);
			if (!plugin_handle) {
				log_fatal("load plugin fail! %s", dlerror());
				exit(1);
			}
		} else {
			plugin_handle = NULL;
		}
	}
	// WARN!!!
	// deamonize() MUST be called before any thread has been created!
	if(is_daemon){
		daemonize();
	}

	{ // ssdb
		ssdb = SSDB::open(*conf, work_dir);
		if(!ssdb){
			log_fatal("could not open work_dir: %s", work_dir.c_str());
			fprintf(stderr, "could not open work_dir: %s\n", work_dir.c_str());
			exit(0);
		}
	}

	{
		serv = new Server(ssdb);
		serv->need_auth = false;		
		if(!password.empty()){
			serv->need_auth = true;
			serv->password = password;
		}
		if (plugin_handle) {
			typedef int (*ssdb_plugin_init)(Server *, const char* opt);
			ssdb_plugin_init c = (ssdb_plugin_init)dlsym(plugin_handle, "ssdb_plugin_init");
			if (!c) {
				log_fatal("load plugin fail! %s", dlerror());
				exit(1);
			}
			int re = c(serv, conf->get_str("server.plugin_opt"));
			if (re) {
				log_fatal("load plugin fail! re=%d", re);
				dlclose(plugin_handle);
				exit(1);
			}
		}
	}

	write_pidfile();
	log_info("ssdb server started.");
}

void write_pidfile(){
	const char *pidfile = conf->get_str("pidfile");
	if(strlen(pidfile)){
		FILE *fp = fopen(pidfile, "w");
		if(!fp){
			log_error("Failed to open pidfile '%s': %s", pidfile, strerror(errno));
			exit(1);
		}
		char buf[128];
		pid_t pid = getpid();
		snprintf(buf, sizeof(buf), "%d", pid);
		log_info("pidfile: %s, pid: %d", pidfile, pid);
		fwrite(buf, 1, strlen(buf), fp);
		fclose(fp);
	}
}

void check_pidfile(){
	const char *pidfile = conf->get_str("pidfile");
	if(strlen(pidfile)){
		if(access(pidfile, F_OK) == 0){
			fprintf(stderr, "Fatal error!\nPidfile %s already exists!\n"
				"You must kill the process and then "
				"remove this file before starting ssdb-server.\n", pidfile);
			exit(1);
		}
	}
}

void remove_pidfile(){
	const char *pidfile = conf->get_str("pidfile");
	if(strlen(pidfile)){
		remove(pidfile);
	}
}
