SSDB C++ API Documentation {#mainpage}
============

@author: [ideawu](http://www.ideawu.com/)

## Build the static library(libssdb-client.a)

Download the SSDB source code from [github](https://github.com/ideawu/ssdb).

    make

The shell commands above will compile the C++ API codes, and generate a `libssdb-client.a` file.

## Sample code

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
		s = client->set("k", "hello ssdb!");
		if(s.ok()){
			printf("k = hello ssdb!\n");
		}else{
			printf("error!\n");
		}
		
		delete client;
		return 0;
	}

Save the codes above into a file named `hello-ssdb.cpp`.

## Compile sample code

If you are under the directory `api/cpp`, compile it like this

	g++ -o hello-ssdb hello-ssdb.cpp libssdb-client.a
	./hello-ssdb

Before you run `hello-ssdb`, you have to start ssdb-server with the default configuration. The output would be like

	k = hello ssdb!

Connect to ssdb-server with `ssdb-cli`, to verify the key `k` is stored with the value "hello ssdb!".

If your `hello-ssdb.cpp` file is not under the directory `api/cpp`, you will compile it like this

	g++ -o hello-ssdb -I<path of api/cpp> hello-ssdb.cpp <path of api/cpp>/libssdb-client.a

