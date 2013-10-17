#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "SSDB.h"

char* readline(char *buf, int num){
	char *ret = fgets(buf, num , stdin);
	if(!ret){
		return NULL;
	}
	if(strlen(ret) > 0){
		ret[strlen(ret) - 1] = '\0';
	}
	return ret;
}

int main(int argc, char **argv){
	printf("Usage: %s [ip] [port]\n", argv[0]);
	const char *ip = (argc >= 2)? argv[1] : "127.0.0.1";
	int port = (argc >= 3)? atoi(argv[2]) : 8888;

	// connect to server
	ssdb::Client *client = ssdb::Client::connect(ip, port);
	if(client == NULL){
		printf("fail to connect to server!\n");
		return 0;
	}

	const std::vector<std::string> *resp;
	client->request("zset", "z", "a", "10");
	resp = client->request("zget", "z", "a");
	printf("%s\n", resp->at(1).c_str());
	
	while(1){
		char buf[8192];
		printf("Press key to read: ");
		char *line = readline(buf, sizeof(buf));
		if(!line){
			break;
		}
		resp = client->request("get", line);
		if(resp == NULL){
			printf("error\n");
			exit(0);
		}
		if(resp->at(0) == "not_found"){
			printf("%s not found!\n", line);
			continue;
		}
		if(resp->at(0) != "ok"){
			printf("server response error: %s\n", resp->at(0).c_str());
			exit(0);
		}
		if(resp->size() != 2){
			printf("bad response!\n");
			exit(0);
		}
		printf("%s = %s\n", line, resp->at(1).c_str());
	}

	delete client;
	return 0;
}
