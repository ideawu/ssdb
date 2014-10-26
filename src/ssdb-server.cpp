#include "include.h"
#include <vector>
#include <string>
#include <list>
#include "version.h"
#include "util/file.h"
#include "util/config.h"
#include "util/daemon.h"
#include "net/server.h"
#include "ssdb.h"
#include "serv.h"

Config *conf = NULL;
NetworkServer *net = NULL;	
std::string pidfile;

void welcome();
void usage(int argc, char **argv);
void init(int argc, char **argv);

void check_pidfile();
void write_pidfile();
void remove_pidfile();

int main(int argc, char **argv){
	welcome();
	init(argc, argv);
	
	SSDB *ssdb = NULL;
	{ // ssdb
		std::string work_dir;
		work_dir = conf->get_str("work_dir");
		if(work_dir.empty()){
			work_dir = ".";
		}
		if(!is_dir(work_dir.c_str())){
			fprintf(stderr, "'%s' is not a directory or not exists!\n", work_dir.c_str());
			exit(1);
		}

		ssdb = SSDB::open(*conf, work_dir);
		if(!ssdb){
			log_fatal("could not open work_dir: %s", work_dir.c_str());
			fprintf(stderr, "could not open work_dir: %s\n", work_dir.c_str());
			exit(1);
		}
	}
	
	SSDBServer *ss = new SSDBServer(ssdb, *conf);
	net->data = ss;
	ss->reg_procs(net);

	write_pidfile();
	log_info("ssdb server started.");
	net->serve();
	remove_pidfile();
	
	delete net;
	delete ss;
	delete ssdb;
	delete conf;
	log_info("ssdb server exit.");
	return 0;
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

void init(int argc, char **argv){
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

	pidfile = conf->get_str("pidfile");
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

	log_info("ssdb-server %s", SSDB_VERSION);
	log_info("conf_file       : %s", conf_file);
	log_info("log_level       : %s", log_level.c_str());
	log_info("log_output      : %s", log_output.c_str());
	log_info("log_rotate_size : %d", log_rotate_size);

	net = new NetworkServer();
	net->init(*conf);

	// WARN!!!
	// deamonize() MUST be called before any thread is created!
	if(is_daemon){
		daemonize();
	}
}

void check_pidfile(){
	if(pidfile.size()){
		if(access(pidfile.c_str(), F_OK) == 0){
			fprintf(stderr, "Fatal error!\nPidfile %s already exists!\n"
				"You must kill the process and then "
				"remove this file before starting the server.\n", pidfile.c_str());
			exit(1);
		}
	}
}

void write_pidfile(){
	if(pidfile.size()){
		FILE *fp = fopen(pidfile.c_str(), "w");
		if(!fp){
			log_error("Failed to open pidfile '%s': %s", pidfile.c_str(), strerror(errno));
			exit(1);
		}
		char buf[128];
		pid_t pid = getpid();
		snprintf(buf, sizeof(buf), "%d", pid);
		log_info("pidfile: %s, pid: %d", pidfile.c_str(), pid);
		fwrite(buf, 1, strlen(buf), fp);
		fclose(fp);
	}
}

void remove_pidfile(){
	if(pidfile.size()){
		remove(pidfile.c_str());
	}
}
