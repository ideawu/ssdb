/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "include.h"

#include <string>
#include <vector>

#include "leveldb/db.h"
#include "leveldb/env.h"
#include "leveldb/options.h"
#include "leveldb/slice.h"
#include "leveldb/iterator.h"

#include "util/log.h"
#include "util/file.h"
#include "util/string_util.h"

void welcome(){
	printf("ssdb-repair - SSDB repair tool\n");
	printf("Copyright (c) 2013-2015 ssdb.io\n");
	printf("\n");
}

void usage(int argc, char **argv){
	printf("Usage:\n");
	printf("    %s leveldb_folder\n", argv[0]);
	printf("\n");
}

int main(int argc, char **argv){
	welcome();

	set_log_level(Logger::LEVEL_MIN);

	if(argc <= 1){
		usage(argc, argv);
		return 0;
	}
	std::string leveldb_folder(argv[1]);

	if(!file_exists(leveldb_folder.c_str())){
		printf("leveldb_folder[%s] not exists!\n", leveldb_folder.c_str());
		return 0;
	}
	
	leveldb::Status status;
	
	leveldb::Logger *logger;
	status = leveldb::Env::Default()->NewLogger("repair.log", &logger);
	if(!status.ok()){
		printf("logger error!\n");
		return 0;
	}
	printf("writing repair log into: repair.log\n");

	leveldb::Options options;
	options.max_file_size = 32 * 1048576; // leveldb 1.20
	options.info_log = logger;
	status = leveldb::RepairDB(leveldb_folder.c_str(), options);
	if(!status.ok()){
		printf("repair leveldb: %s error!\n", leveldb_folder.c_str());
		return 0;
	}
	
	printf("leveldb repaired.\n");
	
	{
		leveldb::DB* db;
		leveldb::Options options;
		leveldb::Status status;
		options.create_if_missing = true;
    	options.max_file_size = 32 * 1048576; // leveldb 1.20
		options.write_buffer_size = 32 * 1024 * 1024;
		options.compression = leveldb::kSnappyCompression;

		status = leveldb::DB::Open(options, leveldb_folder.c_str(), &db);
		if(!status.ok()){
			fprintf(stderr, "ERROR: open leveldb: %s error!\n", leveldb_folder.c_str());
			exit(1);
		}
		printf("compacting data...\n");
		db->CompactRange(NULL, NULL);
	
		{
			std::string val;
			if(db->GetProperty("leveldb.stats", &val)){
				printf("%s\n", val.c_str());
			}
		}

		delete db;
	}

	return 0;
}
