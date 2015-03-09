/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "include.h"
#include "version.h"
#include "net/server.h"
#include "ssdb/ssdb.h"
#include "util/app.h"
#include "serv.h"

#define APP_NAME "ssdb-server"
#define APP_VERSION SSDB_VERSION

class MyApplication : public Application
{
public:
	virtual void usage(int argc, char **argv);
	virtual void welcome();
	virtual void run();
};

void MyApplication::welcome(){
	fprintf(stderr, "%s %s\n", APP_NAME, APP_VERSION);
	fprintf(stderr, "Copyright (c) 2012-2015 ssdb.io\n");
	fprintf(stderr, "\n");
}

void MyApplication::usage(int argc, char **argv){
	printf("Usage:\n");
	printf("    %s [-d] /path/to/ssdb.conf [-s start|stop|restart]\n", argv[0]);
	printf("Options:\n");
	printf("    -d    run as daemon\n");
	printf("    -s    option to start|stop|restart the server\n");
	printf("    -h    show this message\n");
}

void MyApplication::run(){
	Options option;
	option.load(*conf);

	std::string data_db_dir = app_args.work_dir + "/data";
	std::string meta_db_dir = app_args.work_dir + "/meta";

	log_info("ssdb-server %s", APP_VERSION);
	log_info("conf_file        : %s", app_args.conf_file.c_str());
	log_info("log_level        : %s", Logger::shared()->level_name().c_str());
	log_info("log_output       : %s", Logger::shared()->output_name().c_str());
	log_info("log_rotate_size  : %" PRId64, Logger::shared()->rotate_size());

	log_info("main_db          : %s", data_db_dir.c_str());
	log_info("meta_db          : %s", meta_db_dir.c_str());
	log_info("cache_size       : %d MB", option.cache_size);
	log_info("block_size       : %d KB", option.block_size);
	log_info("write_buffer     : %d MB", option.write_buffer_size);
	log_info("max_open_files   : %d", option.max_open_files);
	log_info("compaction_speed : %d MB/s", option.compaction_speed);
	log_info("compression      : %s", option.compression.c_str());
	log_info("binlog           : %s", option.binlog? "yes" : "no");
	log_info("sync_speed       : %d MB/s", conf->get_num("replication.sync_speed"));

	SSDB *data_db = NULL;
	SSDB *meta_db = NULL;
	data_db = SSDB::open(option, data_db_dir);
	if(!data_db){
		log_fatal("could not open data db: %s", data_db_dir.c_str());
		fprintf(stderr, "could not open data db: %s\n", data_db_dir.c_str());
		exit(1);
	}

	meta_db = SSDB::open(Options(), meta_db_dir);
	if(!meta_db){
		log_fatal("could not open meta db: %s", meta_db_dir.c_str());
		fprintf(stderr, "could not open meta db: %s\n", meta_db_dir.c_str());
		exit(1);
	}

	NetworkServer *net = NULL;	
	SSDBServer *server;
	net = NetworkServer::init(*conf);
	server = new SSDBServer(data_db, meta_db, *conf, net);
	
	log_info("pidfile: %s, pid: %d", app_args.pidfile.c_str(), (int)getpid());
	log_info("ssdb server started.");
	net->serve();
	
	delete net;
	delete server;
	delete meta_db;
	delete data_db;

	log_info("%s exit.", APP_NAME);
}

int main(int argc, char **argv){
	MyApplication app;
	return app.main(argc, argv);
}
