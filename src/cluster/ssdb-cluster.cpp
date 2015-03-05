/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "include.h"
#include "version.h"
#include "../net/server.h"
#include "../ssdb/ssdb.h"
#include "serv.h"
#include "app.h"

#define APP_NAME "ssdb-cluster"
#define APP_VERSION SSDB_CLUSTER_VERSION

class MyApplication : public Application
{
public:
	virtual void welcome();
	virtual void run();
};

void MyApplication::welcome(){
	fprintf(stderr, "%s %s\n", APP_NAME, APP_VERSION);
	fprintf(stderr, "Copyright (c) 2012-2015 ssdb.io\n");
	fprintf(stderr, "\n");
}

void MyApplication::run(){
	Options option;
	option.load(*conf);

	log_info("%s %s", APP_NAME, APP_VERSION);
	log_info("conf_file        : %s", app_args.conf_file.c_str());
	log_info("log_level        : %s", log_level_.c_str());
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

	SSDB *data_db = NULL;
	{
		data_db = SSDB::open(option, data_db_dir);
		if(!data_db){
			log_fatal("could not open cluster db: %s", data_db_dir.c_str());
			fprintf(stderr, "could not open cluster db: %s\n", data_db_dir.c_str());
			exit(1);
		}
	}

	NetworkServer *net = NULL;	
	ClusterServer *server = NULL;

	net = NetworkServer::init(*conf, 1, 1);
	server = new ClusterServer(data_db, *conf, net);
	
	log_info("pidfile: %s, pid: %d", app_args.pidfile.c_str(), (int)getpid());
	log_info("%s started.", APP_NAME);
	net->serve();

	delete net;
	delete data_db;
	delete server;

	log_info("%s exit.", APP_NAME);
}

int main(int argc, char **argv){
	MyApplication app;
	return app.main(argc, argv);
}
