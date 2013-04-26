#include "include.h"

#include <string>
#include <vector>

#include "leveldb/db.h"
#include "leveldb/options.h"
#include "leveldb/slice.h"
#include "leveldb/iterator.h"

#include "util/log.h"
#include "util/file.h"
#include "util/strings.h"
#include "link.h"
#include "binlog.h"
#include "t_kv.h"

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

void proc_noop(const Binlog &log, const std::vector<Bytes> &req);
void proc_dump(const Binlog &log, const std::vector<Bytes> &req);
void proc_sync(const Binlog &log, const std::vector<Bytes> &req);


int main(int argc, char **argv){
	set_log_level(Logger::LEVEL_MAX);

	const char *ip = "127.0.0.1";
	int port = 8888;

	// connect to server
	Link *link = Link::connect(ip, port);
	if(link == NULL){
		printf("error connecting to server!\n");
		return 0;
	}

	std::string key;
	key.push_back(DataType::HASH);
	//key.push_back('b');

	link->send("sync", "2", key);

	link->flush();

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
			Binlog log;
			if(log.load(req->at(0).Slice()) == -1){
				continue;
			}
			log_debug("%s", log.dumps().c_str());
			switch(log.type()){
				case BinlogType::NOOP:
					proc_sync(log, *req);
					break;
				case BinlogType::DUMP:
					proc_dump(log, *req);
					break;
				case BinlogType::SYNC:
				case BinlogType::MIRROR:
					proc_sync(log, *req);
					break;
			}
		}
	}

	delete link;
	return 0;
}



void proc_noop(const Binlog &log, const std::vector<Bytes> &req){
}

void proc_dump(const Binlog &log, const std::vector<Bytes> &req){
	switch(log.cmd()){
		case BinlogCommand::BEGIN:
			break;
		case BinlogCommand::END:
			break;
		default:
			proc_sync(log, req);
			break;
	}
}

void proc_sync(const Binlog &log, const std::vector<Bytes> &req){
	switch(log.cmd()){
		case BinlogCommand::KSET:
			if(req.size() != 2){
				//
			}else{
				std::string key;
				if(decode_kv_key(log.key(), &key) == -1){
					//
				}else{
					Bytes val = req[1];
					log_debug("set %s %s",
						hexmem(key.data(), key.size()).c_str(),
						hexmem(val.data(), val.size()).c_str());
					// ssdb->set(key, val);
				}
			}
			break;
		case BinlogCommand::KDEL:
			break;
		case BinlogCommand::HSET:
			break;
		case BinlogCommand::HDEL:
			break;
		case BinlogCommand::ZSET:
			break;
		case BinlogCommand::ZDEL:
			break;
	}
}
