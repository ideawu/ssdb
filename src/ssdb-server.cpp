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

void welcome();
void usage(int argc, char **argv);
void init(int argc, char **argv);
void reg_procs();

int main(int argc, char **argv){
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
	net->serve();
	
	delete net;
	delete ss;
	delete ssdb;
	delete conf;
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
	
	reg_procs();
}

DEF_PROC(get);
DEF_PROC(set);
DEF_PROC(setx);
DEF_PROC(setnx);
DEF_PROC(getset);
DEF_PROC(getbit);
DEF_PROC(setbit);
DEF_PROC(countbit);
DEF_PROC(substr);
DEF_PROC(getrange);
DEF_PROC(strlen);
DEF_PROC(redis_bitcount);
DEF_PROC(del);
DEF_PROC(incr);
DEF_PROC(decr);
DEF_PROC(scan);
DEF_PROC(rscan);
DEF_PROC(keys);
DEF_PROC(exists);
DEF_PROC(multi_exists);
DEF_PROC(multi_get);
DEF_PROC(multi_set);
DEF_PROC(multi_del);

DEF_PROC(hsize);
DEF_PROC(hget);
DEF_PROC(hset);
DEF_PROC(hdel);
DEF_PROC(hincr);
DEF_PROC(hdecr);
DEF_PROC(hclear);
DEF_PROC(hgetall);
DEF_PROC(hscan);
DEF_PROC(hrscan);
DEF_PROC(hkeys);
DEF_PROC(hvals);
DEF_PROC(hlist);
DEF_PROC(hrlist);
DEF_PROC(hexists);
DEF_PROC(multi_hexists);
DEF_PROC(multi_hsize);
DEF_PROC(multi_hget);
DEF_PROC(multi_hset);
DEF_PROC(multi_hdel);

DEF_PROC(zrank);
DEF_PROC(zrrank);
DEF_PROC(zrange);
DEF_PROC(zrrange);
DEF_PROC(zsize);
DEF_PROC(zget);
DEF_PROC(zset);
DEF_PROC(zdel);
DEF_PROC(zincr);
DEF_PROC(zdecr);
DEF_PROC(zclear);
DEF_PROC(zscan);
DEF_PROC(zrscan);
DEF_PROC(zkeys);
DEF_PROC(zlist);
DEF_PROC(zrlist);
DEF_PROC(zcount);
DEF_PROC(zsum);
DEF_PROC(zavg);
DEF_PROC(zexists);
DEF_PROC(zremrangebyrank);
DEF_PROC(zremrangebyscore);
DEF_PROC(multi_zexists);
DEF_PROC(multi_zsize);
DEF_PROC(multi_zget);
DEF_PROC(multi_zset);
DEF_PROC(multi_zdel);
	
DEF_PROC(qsize);
DEF_PROC(qfront);
DEF_PROC(qback);
DEF_PROC(qpush);
DEF_PROC(qpush_front);
DEF_PROC(qpush_back);
DEF_PROC(qpop);
DEF_PROC(qpop_front);
DEF_PROC(qpop_back);
DEF_PROC(qtrim_front);
DEF_PROC(qtrim_back);
DEF_PROC(qfix);
DEF_PROC(qclear);
DEF_PROC(qlist);
DEF_PROC(qrlist);
DEF_PROC(qslice);
DEF_PROC(qrange);
DEF_PROC(qget);
DEF_PROC(qset);

DEF_PROC(dump);
DEF_PROC(sync140);
DEF_PROC(info);
DEF_PROC(compact);
DEF_PROC(key_range);
DEF_PROC(get_key_range);
DEF_PROC(set_key_range);
DEF_PROC(ttl);
DEF_PROC(expire);
DEF_PROC(clear_binlog);


#define PROC(c, f)     net->proc_map.set_proc(#c, f, proc_##c)
#define PROC_KP1(c, f) net->proc_map.set_proc(#c, f, proc_##c)

void reg_procs(){
	PROC_KP1(get, "r");
	PROC_KP1(set, "wt");
	PROC_KP1(del, "wt");
	PROC_KP1(setx, "wt");
	PROC_KP1(setnx, "wt");
	PROC_KP1(getset, "wt");
	PROC_KP1(getbit, "r");
	PROC_KP1(setbit, "wt");
	PROC_KP1(countbit, "r");
	PROC_KP1(substr, "r");
	PROC_KP1(getrange, "r");
	PROC_KP1(strlen, "r");
	PROC_KP1(redis_bitcount, "r");
	PROC_KP1(incr, "wt");
	PROC_KP1(decr, "wt");
	PROC(scan, "rt");
	PROC(rscan, "rt");
	PROC(keys, "rt");
	PROC(exists, "r");
	PROC(multi_exists, "r");
	PROC(multi_get, "rt");
	PROC(multi_set, "wt");
	PROC(multi_del, "wt");

	PROC(hsize, "r");
	PROC(hget, "r");
	PROC(hset, "wt");
	PROC(hdel, "wt");
	PROC(hincr, "wt");
	PROC(hdecr, "wt");
	PROC(hclear, "wt");
	PROC(hgetall, "rt");
	PROC(hscan, "rt");
	PROC(hrscan, "rt");
	PROC(hkeys, "rt");
	PROC(hvals, "rt");
	PROC(hlist, "rt");
	PROC(hrlist, "rt");
	PROC(hexists, "r");
	PROC(multi_hexists, "r");
	PROC(multi_hsize, "r");
	PROC(multi_hget, "rt");
	PROC(multi_hset, "wt");
	PROC(multi_hdel, "wt");

	// because zrank may be extremly slow, execute in a seperate thread
	PROC(zrank, "rt");
	PROC(zrrank, "rt");
	PROC(zrange, "rt");
	PROC(zrrange, "rt");
	PROC(zsize, "r");
	PROC(zget, "rt");
	PROC(zset, "wt");
	PROC(zdel, "wt");
	PROC(zincr, "wt");
	PROC(zdecr, "wt");
	PROC(zclear, "wt");
	PROC(zscan, "rt");
	PROC(zrscan, "rt");
	PROC(zkeys, "rt");
	PROC(zlist, "rt");
	PROC(zrlist, "rt");
	PROC(zcount, "rt");
	PROC(zsum, "rt");
	PROC(zavg, "rt");
	PROC(zremrangebyrank, "wt");
	PROC(zremrangebyscore, "wt");
	PROC(zexists, "r");
	PROC(multi_zexists, "r");
	PROC(multi_zsize, "r");
	PROC(multi_zget, "rt");
	PROC(multi_zset, "wt");
	PROC(multi_zdel, "wt");

	PROC(qsize, "r");
	PROC(qfront, "r");
	PROC(qback, "r");
	PROC(qpush, "wt");
	PROC(qpush_front, "wt");
	PROC(qpush_back, "wt");
	PROC(qpop, "wt");
	PROC(qpop_front, "wt");
	PROC(qpop_back, "wt");
	PROC(qtrim_front, "wt");
	PROC(qtrim_back, "wt");
	PROC(qfix, "wt");
	PROC(qclear, "wt");
	PROC(qlist, "rt");
	PROC(qrlist, "rt");
	PROC(qslice, "rt");
	PROC(qrange, "rt");
	PROC(qget, "r");
	PROC(qset, "wt");

	PROC(clear_binlog, "wt");

	PROC(dump, "b");
	PROC(sync140, "b");
	PROC(info, "r");
	// doing compaction in a reader thread, because we have only one
	// writer thread(for performance reason); we don't want to block writes
	PROC(compact, "rt");
	PROC(key_range, "r"); // deprecated
	//
	PROC(get_key_range, "r");
	// set_key_range must run in the main thread
	PROC(set_key_range, "r");

	PROC(ttl, "r");
	PROC(expire, "wt");
}
