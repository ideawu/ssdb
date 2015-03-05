/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "include.h"
#include <sys/types.h>
#include <signal.h>
#include <vector>
#include <string>
#include <list>
#include "version.h"
#include "../util/file.h"
#include "../util/config.h"
#include "../util/daemon.h"
#include "../net/server.h"
#include "../ssdb/ssdb.h"
#include "serv.h"

struct AppArgs{
	bool is_daemon;
	std::string pidfile;
	std::string conf_file;
	std::string work_dir;
	std::string start_opt;

	AppArgs(){
		is_daemon = false;
		start_opt = "start";
	}
};

AppArgs app_args;
Config *conf = NULL;
NetworkServer *net = NULL;	
Options option;
std::string data_db_dir;

void welcome();
void usage(int argc, char **argv);
void parse_args(int argc, char **argv);
void init();

int read_pid();
void write_pid();
void check_pidfile();
void remove_pidfile();
void kill_process();

int main(int argc, char **argv){
	welcome();
	parse_args(argc, argv);
	init();

	SSDB *data_db = NULL;
	{
		data_db = SSDB::open(option, data_db_dir);
		if(!data_db){
			log_fatal("could not open cluster db: %s", data_db_dir.c_str());
			fprintf(stderr, "could not open cluster db: %s\n", data_db_dir.c_str());
			exit(1);
		}
	}

	net = NetworkServer::init(*conf, 1, 1);

	ClusterServer *ss = new ClusterServer(data_db, *conf, net);

	write_pid();
	log_info("ssdb cluster started.");
	net->serve();
	remove_pidfile();
	
	delete net;
	delete ss;
	delete data_db;
	delete conf;
	log_info("ssdb cluster exit.");
	return 0;
}


void welcome(){
	fprintf(stderr, "ssdb-cluster %s\n", SSDB_VERSION);
	fprintf(stderr, "Copyright (c) 2012-2015 ssdb.io\n");
	fprintf(stderr, "\n");
}

void usage(int argc, char **argv){
	printf("Usage:\n");
	printf("    %s [-d] /path/to/ssdb-cluster.conf [-s start|stop|restart]\n", argv[0]);
	printf("Options:\n");
	printf("    -d    run as daemon\n");
	printf("    -s    option to start|stop|restart the server\n");
	printf("    -h    show this message\n");
}

void parse_args(int argc, char **argv){
	for(int i=1; i<argc; i++){
		std::string arg = argv[i];
		if(arg == "-d"){
			app_args.is_daemon = true;
		}else if(arg == "-v"){
			exit(0);
		}else if(arg == "-h"){
			usage(argc, argv);
			exit(0);
		}else if(arg == "-s"){
			if(argc > i + 1){
				i ++;
				app_args.start_opt = argv[i];
			}else{
				usage(argc, argv);
				exit(1);
			}
			if(app_args.start_opt != "start" && app_args.start_opt != "stop" && app_args.start_opt != "restart"){
				usage(argc, argv);
				fprintf(stderr, "Error: bad argument: '%s'\n", app_args.start_opt.c_str());
				exit(1);
			}
		}else{
			app_args.conf_file = argv[i];
		}
	}

	if(app_args.conf_file.empty()){
		usage(argc, argv);
		exit(1);
	}
}

void init(){
	if(!is_file(app_args.conf_file.c_str())){
		fprintf(stderr, "'%s' is not a file or not exists!\n", app_args.conf_file.c_str());
		exit(1);
	}
	conf = Config::load(app_args.conf_file.c_str());
	if(!conf){
		fprintf(stderr, "error loading conf file: '%s'\n", app_args.conf_file.c_str());
		exit(1);
	}
	{
		std::string conf_dir = real_dirname(app_args.conf_file.c_str());
		if(chdir(conf_dir.c_str()) == -1){
			fprintf(stderr, "error chdir: %s\n", conf_dir.c_str());
			exit(1);
		}
	}

	app_args.pidfile = conf->get_str("pidfile");

	if(app_args.start_opt == "stop"){
		kill_process();
		exit(0);
	}
	if(app_args.start_opt == "restart"){
		if(file_exists(app_args.pidfile)){
			kill_process();
		}
	}
	
	check_pidfile();
	
	std::string log_output;
	std::string log_level;
	int log_rotate_size = 0;
	{ // logger
		log_level = conf->get_str("logger.level");
		if(log_level.empty()){
			log_level = "debug";
		}
		int level = Logger::get_level(log_level.c_str());
		log_rotate_size = conf->get_num("logger.rotate.size");
		log_output = conf->get_str("logger.output");
		if(log_output == ""){
			log_output = "stdout";
		}
		if(log_open(log_output.c_str(), level, true, log_rotate_size) == -1){
			fprintf(stderr, "error opening log file: %s\n", log_output.c_str());
			exit(1);
		}
	}

	app_args.work_dir = conf->get_str("work_dir");
	if(app_args.work_dir.empty()){
		app_args.work_dir = ".";
	}
	if(!is_dir(app_args.work_dir.c_str())){
		fprintf(stderr, "'%s' is not a directory or not exists!\n", app_args.work_dir.c_str());
		exit(1);
	}
	data_db_dir = app_args.work_dir + "/data";

	option.load(*conf);

	log_info("ssdb-cluster %s", SSDB_CLUSTER_VERSION);
	log_info("conf_file        : %s", app_args.conf_file.c_str());
	log_info("log_level        : %s", log_level.c_str());
	log_info("log_output       : %s", log_output.c_str());
	log_info("log_rotate_size  : %d", log_rotate_size);

	log_info("main_db          : %s", data_db_dir.c_str());
	log_info("cache_size       : %d MB", option.cache_size);
	log_info("block_size       : %d KB", option.block_size);
	log_info("write_buffer     : %d MB", option.write_buffer_size);
	log_info("max_open_files   : %d", option.max_open_files);
	log_info("compaction_speed : %d MB/s", option.compaction_speed);
	log_info("compression      : %s", option.compression.c_str());
	log_info("binlog           : %s", option.binlog? "yes" : "no");
	log_info("sync_speed       : %d MB/s", conf->get_num("replication.sync_speed"));

	// WARN!!!
	// deamonize() MUST be called before any thread is created!
	if(app_args.is_daemon){
		daemonize();
	}
}

void check_pidfile(){
	if(app_args.pidfile.size()){
		if(access(app_args.pidfile.c_str(), F_OK) == 0){
			fprintf(stderr, "Fatal error!\nPidfile %s already exists!\n"
				"Kill the running process before you run this command,\n"
				"or use '-s restart' option to restart the server.\n",
				app_args.pidfile.c_str());
			exit(1);
		}
	}
}

int read_pid(){
	if(app_args.pidfile.empty()){
		return -1;
	}
	std::string s;
	file_get_contents(app_args.pidfile, &s);
	if(s.empty()){
		return -1;
	}
	return str_to_int(s);
}

void write_pid(){
	if(app_args.pidfile.empty()){
		return;
	}
	int pid = (int)getpid();
	std::string s = str(pid);
	log_info("pidfile: %s, pid: %d", app_args.pidfile.c_str(), pid);
	int ret = file_put_contents(app_args.pidfile, s);
	if(ret == -1){
		log_error("Failed to write pidfile '%s'(%s)", app_args.pidfile.c_str(), strerror(errno));
		exit(1);
	}
}

void remove_pidfile(){
	if(app_args.pidfile.size()){
		remove(app_args.pidfile.c_str());
	}
}

void kill_process(){
	int pid = read_pid();
	if(pid == -1){
		fprintf(stderr, "could not read pidfile: %s(%s)\n", app_args.pidfile.c_str(), strerror(errno));
		exit(1);
	}
	if(kill(pid, 0) == -1 && errno == ESRCH){
		fprintf(stderr, "process: %d not running\n", pid);
		remove_pidfile();
		return;
	}
	int ret = kill(pid, SIGTERM);
	if(ret == -1){
		fprintf(stderr, "could not kill process: %d(%s)\n", pid, strerror(errno));
		exit(1);
	}
	
	while(file_exists(app_args.pidfile)){
		usleep(100 * 1000);
	}
}
