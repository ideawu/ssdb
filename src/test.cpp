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

int main(int argc, char **argv){
	set_log_level(Logger::LEVEL_MIN);

	const char *ip = "127.0.0.1";
	int port = 8888;

	// connect to server
	Link *link = Link::connect(ip, port);
	if(link == NULL){
		printf("error connecting to server!\n");
		return 0;
	}

	std::string key;
	key.push_back(DataType::KV);
	key.push_back('b');

	link->send("sync", "1", key);

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
			log.load(req->at(0).Slice());
			printf("%s\n", log.dumps().c_str());
		}
	}

	delete link;
	return 0;
}
