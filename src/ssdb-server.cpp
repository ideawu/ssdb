#include "include.h"
#include <vector>
#include <string>
#include <list>

#include "version.h"
#include "ssdb.h"
#include "link.h"
#include "proc.h"
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


volatile bool quit = false;
Config *conf = NULL;
SSDB *ssdb = NULL;
Link *serv_link = NULL;


int main(int argc, char **argv){
	welcome();
	init(argc, argv);
	
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	
	run(argc, argv);

	if(serv_link){
		log_info("shutdown network");
		delete serv_link;
	}
	if(ssdb){
		delete ssdb;
	}
	if(conf){
		delete conf;
	}
	log_info("ssdb server exit.");
	return 0;
}

void run(int argc, char **argv){
	typedef std::vector<Link *> ready_list_t;
	ready_list_t ready_list;
	ready_list_t ready_list_2;
	ready_list_t::iterator it;
	const Fdevents::events_t *events;

	Fdevents select;
	select.set(serv_link->fd(), FDEVENT_IN, 0, serv_link);
	CommandProc proc(ssdb);
	int link_count = 0;

	while(!quit){
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
				link->nodelay();
				link->noblock();
				select.set(link->fd(), FDEVENT_IN, 1, link);
				log_info("new link: %d, link_count: %d", link->fd(), link_count);
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
					log_trace("fd: %d read: %d", link->fd(), len);
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
					log_trace("fd: %d write: %d", link->fd(), len);
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

		ready_list_2.clear();
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

			Response resp;
			int status = proc.proc(*link, *req, &resp);
			if(status == PROC_ERROR){
				log_info("fd: %d, proc error, delete link", link->fd());
				link_count --;
				select.del(link->fd());
				delete link;
				continue;
			}else if(status == PROC_BACKEND){
				link_count --;
				select.del(link->fd());
				continue;
			}

			if(link->send(resp) == -1){
				log_info("fd: %d, send error, delete link", link->fd());
				link_count --;
				select.del(link->fd());
				delete link;
				continue;
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
					ready_list_2.push_back(link);
				}
			}
		} // end foreach ready link
		ready_list.swap(ready_list_2);
	}
}


void welcome(){
	printf("ssdb %s\n", SSDB_VERSION);
	printf("Copyright (c) 2012 ideawu.com\n");
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

	log_info("starting ssdb server...");
	log_info("conf_file  : %s", conf_file);
	log_info("work_dir   : %s", work_dir.c_str());
	log_info("log_level  : %s", conf->get_str("logger.level"));
	log_info("log_output : %s", log_output.c_str());
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

	log_info("ssdb server started.");
}

