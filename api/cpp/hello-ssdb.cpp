#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "SSDB.h"

int main(int argc, char **argv){
	const char *ip = (argc >= 2)? argv[1] : "127.0.0.1";
	int port = (argc >= 3)? atoi(argv[2]) : 8888;

	ssdb::Client *client = ssdb::Client::connect(ip, port);
	if(client == NULL){
		printf("fail to connect to server!\n");
		return 0;
	}

	const std::vector<std::string> *resp;
	resp = client->request("set", "k", "hello ssdb!");
	if(resp && resp->size() > 0 && resp->at(0) == "ok"){
		printf("k = hello ssdb!\n");
	}else{
		printf("error!\n");
	}

	delete client;
	return 0;
}
