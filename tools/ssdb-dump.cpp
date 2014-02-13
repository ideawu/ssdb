#include "include.h"
#include <sys/types.h>
#include <sys/stat.h>

#include <string>
#include <vector>

#include "leveldb/db.h"
#include "leveldb/options.h"
#include "leveldb/slice.h"
#include "leveldb/iterator.h"

#include "link.h"
#include "include.h"
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
	welcome();

	set_log_level(Logger::LEVEL_MIN);

	if(argc <= 3){
		usage(argc, argv);
		return 0;
	}
	char *ip = argv[1];
	int port = atoi(argv[2]);
	char *output_folder = argv[3];

	if(file_exists(output_folder)){
		printf("output_folder[%s] exists!\n", output_folder);
		return 0;
	}
	if(mkdir(output_folder, 0777) == -1){
		perror("error create backup directory!");
		return 0;
	}

	std::string data_dir = "";
	data_dir.append(output_folder);
	data_dir.append("/data");
	
	{
		std::string meta_dir = "";
		meta_dir.append(output_folder);
		meta_dir.append("/meta");

		int ret;
		ret = mkdir(meta_dir.c_str(), 0755);
		if(ret == -1){
			fprintf(stderr, "error creating meta dir\n");
			exit(0);
		}
	}

	// connect to server
	Link *link = Link::connect(ip, port);
	if(link == NULL){
		fprintf(stderr, "error connecting to server!\n");
		return 0;
	}

	link->send("dump", "", "", "2147483647");
	link->flush();

	leveldb::DB* db;
	leveldb::Options options;
	leveldb::Status status;
	options.create_if_missing = true;
	options.write_buffer_size = 32 * 1024 * 1024;
	options.compression = leveldb::kSnappyCompression;

	status = leveldb::DB::Open(options, data_dir.c_str(), &db);
	if(!status.ok()){
		printf("open leveldb: %s error!\n", output_folder);
		return 0;
	}

	int dump_count = 0;
	while(1){
		const std::vector<Bytes> *req = link->recv();
		if(req == NULL){
			printf("error\n");
			break;
		}else if(req->empty()){
			if(link->read() <= 0){
				printf("read end\n");
				break;
			}
		}else{
			Bytes cmd = req->at(0);
			if(cmd == "begin"){
				printf("recv begin...\n");
			}else if(cmd == "end"){
				printf("recv end\n\n");
				break;
			}else if(cmd == "set"){
				/*
				std::string s = serialize_req(*req);
				printf("%s\n", s.c_str());
				*/

				if(req->size() != 3){
					printf("invalid set params!\n");
					break;
				}
				Bytes key = req->at(1);
				Bytes val = req->at(2);
				if(key.size() == 0 || key.data()[0] == DataType::SYNCLOG){
					continue;
				}
				
				leveldb::Slice k = key.Slice();
				leveldb::Slice v = val.Slice();
				status = db->Put(leveldb::WriteOptions(), k, v);
				if(!status.ok()){
					printf("put leveldb error!\n");
					break;
				}

				dump_count ++;
				if((int)log10(dump_count - 1) != (int)log10(dump_count)){
					printf("received %d entry(s)\n", dump_count);
				}
			}else{
				printf("error: unknown command %s\n", std::string(cmd.data(), cmd.size()).c_str());
				break;
			}
		}
	}
	printf("total dumped %d entry(s)\n", dump_count);

	/*
	printf("checking data...\n");
	leveldb::Iterator *it;
	it = db->NewIterator(leveldb::ReadOptions());
	int save_count = 0;
	for(it->SeekToFirst(); it->Valid(); it->Next()){
		save_count ++;
		//std::string k = hexmem(it->key().data(), it->key().size());
		//std::string v = hexmem(it->value().data(), it->value().size());
		//printf("%d %s : %s", save_count, k.c_str(), v.c_str());
	}
	if(dump_count != save_count){
		printf("checking failed! dumped: %d, saved: %d\n", dump_count, save_count);
	}else{
		printf("checking OK.\n");
		printf("\n");
	}
	*/

	{
		std::string val;
		if(db->GetProperty("leveldb.stats", &val)){
			fprintf(stderr, "%s\n", val.c_str());
		}
	}

	fprintf(stderr, "compacting data...\n");
	db->CompactRange(NULL, NULL);
	
	{
		std::string val;
		if(db->GetProperty("leveldb.stats", &val)){
			fprintf(stderr, "%s\n", val.c_str());
		}
	}

	printf("backup has been made to folder: %s\n", output_folder);

	delete link;
	delete db;
	return 0;
}
