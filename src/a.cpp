#include "include.h"

#include <string>
#include <vector>

#include "leveldb/db.h"
#include "leveldb/options.h"
#include "leveldb/slice.h"
#include "leveldb/iterator.h"

#include "link.h"
#include "util/log.h"
#include "util/file.h"
#include "util/strings.h"

template<class T>
static std::string serialize_req(T &req){
	std::string ret;
	char buf[50];
	for(int i=0; i<req.size(); i++){
		if(i >= 5 && i < req.size() - 1){
			sprintf(buf, "[%d more...]", (int)req.size() - i - 1);
			ret.append(buf);
			break;
		}
		if(((req[0] == "get" || req[0] == "set") && i == 1) || req[i].size() < 30){
			std::string h = hexmem(req[i].data(), req[i].size());
			ret.append(h);
		}else{
			sprintf(buf, "[%d bytes]", (int)req[i].size());
			ret.append(buf);
		}
		if(i < req.size() - 1){
			ret.append(" ");
		}
	}
	return ret;
}

void welcome(){
	printf("ssdb-dump - SSDB backup command\n");
	printf("Copyright (c) 2012 ideawu.com\n");
	printf("\n");
}

void usage(int argc, char **argv){
	printf("Usage:\n");
	printf("    %s ip port output_folder\n", argv[0]);
	printf("\n");
	printf("Options:\n");
	printf("    ip - ssdb server ip address\n");
	printf("    port - ssdb server port number\n");
	printf("    output_folder - local backup folder that will be created\n");
}

int main(int argc, char **argv){
	set_log_level(Logger::LEVEL_MIN);

	std::string data_dir = "a";
	
	leveldb::DB* db;
	leveldb::Options options;
	leveldb::Status status;
	options.create_if_missing = true;
	status = leveldb::DB::Open(options, data_dir.c_str(), &db);
	if(!status.ok()){
		return 0;
	}

	db->Put(leveldb::WriteOptions(), "a", "a");
	db->Put(leveldb::WriteOptions(), "b", "b");

	leveldb::Iterator *it;
	it = db->NewIterator(leveldb::ReadOptions());
	int save_count = 0;
	for(it->SeekToFirst(); it->Valid(); it->Next()){
		save_count ++;
		std::string k = hexmem(it->key().data(), it->key().size());
		std::string v = hexmem(it->value().data(), it->value().size());
		printf("%d %s : %s\n", save_count, k.c_str(), v.c_str());
		db->Get(leveldb::ReadOptions(), it->key(), &v);
		printf("%d %s : %s\n", save_count, k.c_str(), v.c_str());
		if(it->key() == "a"){
			db->Put(leveldb::WriteOptions(), "b", "c");
		}
	}

	delete db;
	return 0;
}
