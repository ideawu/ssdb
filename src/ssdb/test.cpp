#include <string>
#include "ssdb.h"
#include "../util/log.h"
#include "../util/config.h"

int main(int argc, char **argv){
	set_log_level(Logger::LEVEL_TRACE);
	std::string work_dir = "./tmp";
	Option opt;

	SSDB *ssdb = NULL;
	ssdb = SSDB::open(opt, work_dir);
	if(!ssdb){
		log_fatal("could not open work_dir: %s", work_dir.c_str());
		fprintf(stderr, "could not open work_dir: %s\n", work_dir.c_str());
		exit(1);
	}

	std::string key, val;
	key = "a";

	ssdb->get(key, &val);
	int num = str_to_int(val) + 1;
	ssdb->set(key, int_to_str(num));
	ssdb->get(key, &val);
	
	log_debug("%s", val.c_str());
	delete ssdb;
}
