#include "option.h"
#include "../util/strings.h"

Option::Option(){
	Config c;
	this->load(c);
}

void Option::load(const Config &conf){
	cache_size = (size_t)conf.get_num("leveldb.cache_size");
	max_open_files = (size_t)conf.get_num("leveldb.max_open_files");
	write_buffer_size = (size_t)conf.get_num("leveldb.write_buffer_size");
	block_size = (size_t)conf.get_num("leveldb.block_size");
	compaction_speed = conf.get_num("leveldb.compaction_speed");
	compression = conf.get_str("leveldb.compression");
	std::string binlog = conf.get_str("replication.binlog");

	strtolower(&compression);
	if(compression != "no"){
		compression = "yes";
	}
	strtolower(&binlog);
	if(binlog != "yes"){
		this->binlog = false;
	}else{
		this->binlog = true;
	}

	if(cache_size <= 0){
		cache_size = 8;
	}
	if(write_buffer_size <= 0){
		write_buffer_size = 4;
	}
	if(block_size <= 0){
		block_size = 4;
	}
	if(max_open_files <= 0){
		max_open_files = cache_size / 1024 * 30;
		if(max_open_files < 100){
			max_open_files = 100;
		}
		if(max_open_files > 1000){
			max_open_files = 1000;
		}
	}
}
