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
	log_info("ssdb server exit.");
	return 0;
}

int proc_result(ProcJob &job, Fdevents &fdes, ready_list_t &ready_list){	
	Link *link = job.link;
			
	if(job.result == PROC_ERROR){
		log_info("fd: %d, proc error, delete link", link->fd());
		fdes.del(link->fd());
		delete link;
		return PROC_ERROR;
	}
	if(job.cmd){
		job.cmd->calls += 1;
		job.cmd->time_wait += job.time_wait;
		job.cmd->time_proc += job.time_proc;
	}

	if(!link->output->empty()){
		//log_trace("add %d to fdes.out", link->fd());
		fdes.set(link->fd(), FDEVENT_OUT, 1, link);
		if(fdes.isset(link->fd(), FDEVENT_IN)){
			//log_trace("delete %d from fdes.in", link->fd());
			fdes.clr(link->fd(), FDEVENT_IN);
		}
	}else{
		if(link->input->empty()){
			if(!fdes.isset(link->fd(), FDEVENT_IN)){
				//log_trace("add %d to fdes.in", link->fd());
				fdes.set(link->fd(), FDEVENT_IN, 1, link);
			}
		}else{
			if(fdes.isset(link->fd(), FDEVENT_IN)){
				//log_trace("delete %d from fdes.in", link->fd());
				fdes.clr(link->fd(), FDEVENT_IN);
			}
			ready_list.push_back(link);
		}
	}
	return PROC_OK;
}

void run(int argc, char **argv){
	ready_list_t ready_list;
	ready_list_t ready_list_2;
	ready_list_t::iterator it;
	const Fdevents::events_t *events;
	Server serv(ssdb);

	Fdevents fdes;
	fdes.set(serv_link->fd(), FDEVENT_IN, 0, serv_link);
	fdes.set(serv.reader->fd(), FDEVENT_IN, 0, serv.reader);
	fdes.set(serv.writer->fd(), FDEVENT_IN, 0, serv.writer);
	
	int link_count = 0;
	uint32_t last_ticks = g_ticks;
	
	while(!quit){
		// status report
		if((uint32_t)(g_ticks - last_ticks) >= STATUS_REPORT_TICKS){
			last_ticks = g_ticks;
			log_info("ssdb working, links: %d", link_count);
		}
		
		ready_list.clear();
		ready_list_2.clear();
		
		if(!ready_list.empty()){
			// ready_list not empty, so we should return immediately
			events = fdes.wait(0);
		}else{
			events = fdes.wait(50);
		}
		if(events == NULL){
			log_fatal("events.wait error: %s", strerror(errno));
			break;
		}
		
		for(int i=0; i<(int)events->size(); i++){
			const Fdevent *fde = events->at(i);
			if(fde->data.ptr == serv_link){
				Link *link = serv_link->accept();
				if(link == NULL){
					log_error("accept failed! %s", strerror(errno));
					continue;
				}
				if(!ip_filter->check_pass(link->remote_ip)){
					log_debug("ip_filter deny link from %s:%d",
						link->remote_ip, link->remote_port);
					delete link;
					continue;
				}
				link_count ++;				
				log_debug("new link from %s:%d, fd: %d, link_count: %d",
					link->remote_ip, link->remote_port, link->fd(), link_count);
				
				link->nodelay();
				link->noblock();
				link->create_time = millitime();
				link->active_time = link->create_time;
				fdes.set(link->fd(), FDEVENT_IN, 1, link);
			}else if(fde->data.ptr == serv.reader || fde->data.ptr == serv.writer){
				WorkerPool<Server::ProcWorker, ProcJob> *worker = (WorkerPool<Server::ProcWorker, ProcJob> *)fde->data.ptr;
				ProcJob job;
				if(worker->pop(&job) == 0){
					log_fatal("reading result from workers error!");
					exit(0);
				}
				if(proc_result(job, fdes, ready_list_2) == PROC_ERROR){
					link_count --;
				}
			}else{
				Link *link = (Link *)fde->data.ptr;
				// 不能同时监听读和写事件, 只能监听其中一个
				if(fde->events & FDEVENT_ERR){
					log_info("fd: %d error, delete link", link->fd());
					link_count --;
					fdes.del(link->fd());
					delete link;
				}else if(fde->events & FDEVENT_IN){
					int len = link->read();
					//log_trace("fd: %d read: %d", link->fd(), len);
					if(len <= 0){
						log_debug("fd: %d, read: %d, delete link", link->fd(), len);
						link_count --;
						fdes.del(link->fd());
						delete link;
					}else{
						ready_list.push_back(link);
					}
				}else if(fde->events & FDEVENT_OUT){
					int len = link->write();
					//log_trace("fd: %d write: %d", link->fd(), len);
					if(len <= 0){
						log_debug("fd: %d, write: %d, delete link", link->fd(), len);
						link_count --;
						fdes.del(link->fd());
						delete link;
					}else if(link->output->empty()){
						//log_trace("delete %d from fdes.out", link->fd());
						fdes.clr(link->fd(), FDEVENT_OUT);
						if(!link->input->empty()){
							ready_list.push_back(link);
						}else{
							//log_trace("add %d to fdes.in", link->fd());
							fdes.set(link->fd(), FDEVENT_IN, 1, link);
						}
					}else{
						//log_trace("%d", link->output->size());
					}
				}
			}
		}

		for(it = ready_list.begin(); it != ready_list.end(); it ++){
			Link *link = *it;

			const Request *req = link->recv();
			if(req == NULL){
				log_warn("fd: %d, link parse error, delete link", link->fd());
				link_count --;
				fdes.del(link->fd());
				delete link;
				continue;
			}
			if(req->empty()){
				if(!fdes.isset(link->fd(), FDEVENT_IN)){
					//log_trace("add %d to fdes.in", link->fd());
					fdes.set(link->fd(), FDEVENT_IN, 1, link);
				}
				continue;
			}
			
			link->active_time = millitime();

			ProcJob job;
			job.link = link;
			serv.proc(&job);
			if(job.result == PROC_THREAD){
				fdes.del(link->fd());
				continue;
			}
			if(job.result == PROC_BACKEND){
				fdes.del(link->fd());
				link_count --;
				continue;
			}
			
			if(proc_result(job, fdes, ready_list_2) == PROC_ERROR){
				link_count --;
			}
		} // end foreach ready link
		ready_list.swap(ready_list_2);
	}
}


void welcome(){
	fprintf(stderr, "ssdb %s\n", SSDB_VERSION);
	fprintf(stderr, "Copyright (c) 2012-2014 ideawu.com\n");
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
		fprintf(stderr, "error loading conf file: '%s'", conf_file);
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
			fprintf(stderr, "error opening log file: %s", log_output.c_str());
			exit(1);
		}
	}

	log_info("ssdb-server %s", SSDB_VERSION);
	log_info("conf_file       : %s", conf_file);
	log_info("work_dir        : %s", work_dir.c_str());
	log_info("log_level       : %s", conf->get_str("logger.level"));
	log_info("log_output      : %s", log_output.c_str());
	log_info("log_rotate_size : %d", log_rotate_size);
	
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
			fprintf(stderr, "error opening server socket! %s", strerror(errno));
			exit(1);
		}
		log_info("server listen on: %s:%d", ip, port);
		log_info("ssdb server started.");
		fprintf(stderr, "ssdb server started, listen on %s:%d\n", ip, port);
	}

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

	write_pidfile();
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
