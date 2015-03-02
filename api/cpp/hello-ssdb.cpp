#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "SSDB_client.h"

int main(int argc, char **argv){
	const char *ip = (argc >= 2)? argv[1] : "127.0.0.1";
	int port = (argc >= 3)? atoi(argv[2]) : 8888;

	ssdb::Client *client = ssdb::Client::connect(ip, port);
	if(client == NULL){
		printf("fail to connect to server!\n");
		return 0;
	}

	ssdb::Status s;
	std::string val;
	std::vector<std::string> vals;

	// set and get
	s = client->set("k", "hello ssdb!");
	if(s.ok()){
		printf("k = hello ssdb!\n");
	}else{
		printf("error!\n");
	}
	s = client->get("k", &val);
	printf("length: %d\n", (int)val.size());

	// qpush, qslice, qpop
	s = client->qpush("k", "hello1!");
	if(s.ok()){
		printf("qpush k = hello1!\n");
	}else{
		printf("error!\n");
	}
	s = client->qpush("k", "hello2!");
	if(s.ok()){
		printf("qpush k = hello2!\n");
	}else{
		printf("error!\n");
	}
	s = client->qslice("k", 0, 1, &vals);
	if(s.ok()){
		printf("qslice 0 1\n");
		for(int i = 0; i < (int)vals.size(); i++){
			printf("    %d %s\n", i, vals[i].c_str());
		}
	}else{
		printf("error!\n");
	}
	s = client->qpop("k", &val);
	if(s.ok()){
		printf("qpop k = %s\n", val.c_str());
	}else{
		printf("error!\n");
	}
	s = client->qpop("k", &val);
	if(s.ok()){
		printf("qpop k = %s\n", val.c_str());
	}else{
		printf("error!\n");
	}

	delete client;
	return 0;
}
