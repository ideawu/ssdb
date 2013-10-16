#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "link.h"
//#include "util/log.h"

int main(int argc, char **argv){
	if(argc < 3){
		printf("Usage: %s ip port\n", argv[0]);
		return 0;
	}
	char *ip = argv[1];
	int port = atoi(argv[2]);

	// connect to server
	Link *link = Link::connect(ip, port);
	if(link == NULL){
		printf("error connecting to server!\n");
		return 0;
	}
	
	while(1){
		char buf[8192];
		printf("Press key to read: ");
		char *line = gets(buf);
		if(!line){
			break;
		}
		const std::vector<Bytes> *req;
		req = link->request("get", line);
		if(req == NULL){
			printf("error\n");
			exit(0);
		}
		if(req->at(0) == "not_found"){
			printf("%s not found!\n", line);
			continue;
		}
		if(req->at(0) != "ok"){
			printf("server response error: %s\n", req->at(0).String().c_str());
			exit(0);
		}
		if(req->size() != 2){
			printf("bad response!\n");
		}
		printf("%s = %s\n", line, req->at(1).String().c_str());
	}
	
	delete link;
	return 0;
}
