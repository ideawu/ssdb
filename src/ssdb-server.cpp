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

void welcome();
void usage(int argc, char **argv);
void signal_handler(int sig);
void init(int argc, char **argv);
void run(int argc, char **argv);
void write_pidfile();
void check_pidfile();
void check_pidfile();
void remove_pidfile();

volatile bool quit = false;
Config *conf = NULL;
SSDB *ssdb = NULL;
Link *serv_link = NULL;

typedef std::vector<Link *> ready_list_t;


int main(int argc, char **argv){
	welcome();
	init(argc, argv);
	
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	
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

int proc_result(ProcJob &job, Fdevents &select, ready_list_t &ready_list){	
	Link *link = job.link;
			
	if(job.result == PROC_ERROR){
		log_info("fd: %d, proc error, delete link", link->fd());
		select.del(link->fd());
		delete link;
		return PROC_ERROR;
	}
	if(job.cmd){
		job.cmd->calls += 1;
		job.cmd->time_wait += job.time_wait;
		job.cmd->time_proc += job.time_proc;
	}

	if(!link->output->empty()){
		//log_trace("add %d to select.out", link->fd());
		select.set(link->fd(), FDEVENT_OUT, 1, link);
		if(select.isset(link->fd(), FDEVENT_IN)){
			//log_trace("delete %d from select.in", link->fd());
			select.clr(link->fd(), FDEVENT_IN);
		}
	}else{
		if(link->input->empty()){
			if(!select.isset(link->fd(), FDEVENT_IN)){
				//log_trace("add %d to select.in", link->fd());
				select.set(link->fd(), FDEVENT_IN, 1, link);
			}
		}else{
			if(select.isset(link->fd(), FDEVENT_IN)){
				//log_trace("delete %d from select.in", link->fd());
				select.clr(link->fd(), FDEVENT_IN);
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

	Fdevents select;
	select.set(serv_link->fd(), FDEVENT_IN, 0, serv_link);
	//select.set(serv.writer.fd(), FDEVENT_IN, 0, &serv);
	
	int link_count = 0;
	while(!quit){
		ready_list.clear();
		ready_list_2.clear();
		
		if(!ready_list.empty()){
			// give links that are not in ready_list a chance
			events = select.wait(0);
		}else{
			events = select.wait(500);
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
					log_error("accept fail!");
					continue;
				}
				link_count ++;
				log_info("new link from %s:%d, fd: %d, link_count: %d",
					link->remote_ip, link->remote_port, link->fd(), link_count);
				
				link->nodelay();
				//link->noblock();
				link->create_time = millitime();
				link->active_time = link->create_time;
				select.set(link->fd(), FDEVENT_IN, 1, link);
			}else if(fde->data.ptr == &serv){
				/*
				ProcJob job;
				if(serv.writer.pop(&job) == 0){
					log_fatal("reading result from workers error!");
					exit(0);
				}
				if(proc_result(job, select, ready_list_2) == PROC_ERROR){
					link_count --;
				}
				*/
			}else{
				Link *link = (Link *)fde->data.ptr;
				// 不能同时监听读和写事件, 只能监听其中一个
				if(fde->events & FDEVENT_ERR){
					log_info("fd: %d error, delete link", link->fd());
					link_count --;
					select.del(link->fd());
					delete link;
				}else if(fde->events & FDEVENT_IN){
					int len = link->read();
					//log_trace("fd: %d read: %d", link->fd(), len);
					if(len <= 0){
						log_info("fd: %d, read: %d, delete link", link->fd(), len);
						link_count --;
						select.del(link->fd());
						delete link;
					}else{
						ready_list.push_back(link);
					}
				}else if(fde->events & FDEVENT_OUT){
					int len = link->write();
					//log_trace("fd: %d write: %d", link->fd(), len);
					if(len <= 0){
						log_info("fd: %d, write: %d, delete link", link->fd(), len);
						link_count --;
						select.del(link->fd());
						delete link;
					}else if(link->output->empty()){
						//log_trace("delete %d from select.out", link->fd());
						select.clr(link->fd(), FDEVENT_OUT);
						if(!link->input->empty()){
							ready_list.push_back(link);
						}else{
							//log_trace("add %d to select.in", link->fd());
							select.set(link->fd(), FDEVENT_IN, 1, link);
						}
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
				select.del(link->fd());
				delete link;
				continue;
			}
			if(req->empty()){
				if(!select.isset(link->fd(), FDEVENT_IN)){
					//log_trace("add %d to select.in", link->fd());
					select.set(link->fd(), FDEVENT_IN, 1, link);
				}
				continue;
			}
	
			ProcJob job;
			job.link = link;
			serv.proc(&job);
			if(job.result == PROC_BACKEND){
				link_count --;
				select.del(link->fd());
				continue;
			}
			link->active_time = millitime();
			
			if(proc_result(job, select, ready_list_2) == PROC_ERROR){
				link_count --;
			}
		} // end foreach ready link
		ready_list.swap(ready_list_2);
	}
}


void welcome(){
	printf("ssdb %s\n", SSDB_VERSION);
	printf("Copyright (c) 2012-2013 ideawu.com\n");
	printf("\n");
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
		case SIGINT:
			quit = true;
			break;
	}
}

void init(int argc, char **argv){
	if(argc < 2){
		usage(argc, argv);
		exit(0);
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
		exit(0);
	}

	if(!is_file(conf_file)){
		fprintf(stderr, "'%s' is not a file or not exists!\n", conf_file);
		exit(0);
	}

	conf = Config::load(conf_file);
	if(!conf){
		fprintf(stderr, "error loading conf file: '%s'", conf_file);
		exit(0);
	}
	{
		std::string conf_dir = real_dirname(conf_file);
		if(chdir(conf_dir.c_str()) == -1){
			fprintf(stderr, "error chdir: %s\n", conf_dir.c_str());
			exit(0);
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
			exit(0);
		}
		/*
		if(chdir(work_dir.c_str()) == -1){
			fprintf(stderr, "error chdir: %s\n", work_dir.c_str());
			exit(0);
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
			fprintf(stderr, "error open log file: %s", log_output.c_str());
			exit(0);
		}
	}

	log_info("ssdb-server %s", SSDB_VERSION);
	log_info("conf_file       : %s", conf_file);
	log_info("work_dir        : %s", work_dir.c_str());
	log_info("log_level       : %s", conf->get_str("logger.level"));
	log_info("log_output      : %s", log_output.c_str());
	log_info("log_rotate_size : %d", log_rotate_size);

	if(is_daemon){
		daemonize();
	}

	{ // ssdb
		ssdb = SSDB::open(*conf, work_dir);
	}

	{ // server
		const char *ip = conf->get_str("server.ip");
		short port = (short)conf->get_num("server.port");

		serv_link = Link::listen(ip, port);
		if(serv_link == NULL){
			log_fatal("error opening server socket! %s", strerror(errno));
			exit(0);
		}
		log_info("server listen on: %s:%d", ip, port);
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
			exit(0);
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
			exit(0);
		}
	}
}

void remove_pidfile(){
	const char *pidfile = conf->get_str("pidfile");
	if(strlen(pidfile)){
		remove(pidfile);
	}
}
