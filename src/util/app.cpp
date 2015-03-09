#include "app.h"
#include "log.h"
#include "file.h"
#include "config.h"
#include "daemon.h"
#include "strings.h"
#include <stdio.h>

int Application::main(int argc, char **argv){
	conf = NULL;

	welcome();
	parse_args(argc, argv);
	init();

	write_pid();
	run();
	remove_pidfile();
	
	delete conf;
	return 0;
}

void Application::usage(int argc, char **argv){
	printf("Usage:\n");
	printf("    %s [-d] /path/to/app.conf [-s start|stop|restart]\n", argv[0]);
	printf("Options:\n");
	printf("    -d    run as daemon\n");
	printf("    -s    option to start|stop|restart the server\n");
	printf("    -h    show this message\n");
}

void Application::parse_args(int argc, char **argv){
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

void Application::init(){
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
	
	{ // logger
		std::string log_output;
		std::string log_level_;
		int64_t log_rotate_size;

		log_level_ = conf->get_str("logger.level");
		strtolower(&log_level_);
		if(log_level_.empty()){
			log_level_ = "debug";
		}
		int level = Logger::get_level(log_level_.c_str());
		log_rotate_size = conf->get_int64("logger.rotate.size");
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

	// WARN!!!
	// deamonize() MUST be called before any thread is created!
	if(app_args.is_daemon){
		daemonize();
	}
}

int Application::read_pid(){
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

void Application::write_pid(){
	if(app_args.pidfile.empty()){
		return;
	}
	int pid = (int)getpid();
	std::string s = str(pid);
	int ret = file_put_contents(app_args.pidfile, s);
	if(ret == -1){
		log_error("Failed to write pidfile '%s'(%s)", app_args.pidfile.c_str(), strerror(errno));
		exit(1);
	}
}

void Application::check_pidfile(){
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

void Application::remove_pidfile(){
	if(app_args.pidfile.size()){
		remove(app_args.pidfile.c_str());
	}
}

void Application::kill_process(){
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

