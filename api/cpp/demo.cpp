#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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
	
	ssdb::Status s;
	std::string hash = "h";
	std::string zset = "z";
	std::string key = "k";
	std::string test_val = "test_val";
	std::string val;
	
	printf("\n");
	{
		s = client->setx(key, "test_val", 3);
		assert(s.ok());

		s = client->set(key, "test_val");
		assert(s.ok());

		s = client->get(key, &val);
		assert(s.ok() && (val == test_val));
		printf("%s = %s\n", key.c_str(), val.c_str());

		s = client->del(key);
		assert(s.ok());

		s = client->get(key, &val);
		assert(s.not_found());
		
		int64_t ret;
		s = client->incr(key, 3, &ret);
		assert(s.ok() && (ret == 3));
		s = client->incr(key, -1, &ret);
		assert(s.ok() && (ret == 2));
		
		std::vector<std::string> list;
		s = client->keys("", "", 2, &list);
		assert(s.ok() && list.size() <= 2);
		
		list.clear();
		s = client->scan("", "", 2, &list);
		assert(s.ok() && list.size() <= 4);
		for(int i=0; i<list.size(); i++){
			if(i%2 == 0){
				printf("%s=", list[i].c_str());
			}else{
				printf("%s, ", list[i].c_str());
			}
		}
		printf("\n");
		
		list.clear();
		s = client->rscan("", "", 2, &list);
		assert(s.ok() && list.size() <= 4);
		for(int i=0; i<list.size(); i++){
			if(i%2 == 0){
				printf("%s=", list[i].c_str());
			}else{
				printf("%s, ", list[i].c_str());
			}
		}
		printf("\n");
	}
	
	printf("\n");
	{
		s = client->hset(hash, key, "test_val");
		assert(s.ok());

		s = client->hget(hash, key, &val);
		assert(s.ok() && (val == test_val));
		printf("%s = %s\n", key.c_str(), val.c_str());

		s = client->hdel(hash, key);
		assert(s.ok());

		s = client->hget(hash, key, &val);
		assert(s.not_found());
		
		int64_t ret;
		ret = -1;
		s = client->hsize(hash, &ret);
		assert(s.ok() && (ret != -1));
		s = client->hincr(hash, key, 3, &ret);
		assert(s.ok() && (ret == 3));
		s = client->hincr(hash, key, -1, &ret);
		assert(s.ok() && (ret == 2));
		
		std::vector<std::string> list;
		s = client->hkeys(hash, "", "", 2, &list);
		assert(s.ok() && list.size() <= 2);
		
		list.clear();
		s = client->hscan(hash, "", "", 2, &list);
		assert(s.ok() && list.size() <= 4);
		for(int i=0; i<list.size(); i++){
			if(i%2 == 0){
				printf("%s=", list[i].c_str());
			}else{
				printf("%s, ", list[i].c_str());
			}
		}
		printf("\n");
		
		list.clear();
		s = client->hrscan(hash, "", "", 2, &list);
		assert(s.ok() && list.size() <= 4);
		for(int i=0; i<list.size(); i++){
			if(i%2 == 0){
				printf("%s=", list[i].c_str());
			}else{
				printf("%s, ", list[i].c_str());
			}
		}
		printf("\n");

		ret = -1;
		s = client->hclear(hash, &ret);
		assert(s.ok() && ret != -1);
	}
	
	printf("\n");
	{
		std::vector<std::string> list;
		int64_t test_score = 100;
		int64_t score;
		s = client->zset(zset, key, test_score);
		assert(s.ok());

		s = client->zget(zset, key, &score);
		assert(s.ok() && (score == test_score));
		printf("%s = %d\n", key.c_str(), (int)score);

		s = client->zdel(zset, key);
		assert(s.ok());

		s = client->zget(zset, key, &score);
		assert(s.not_found());
		
		int64_t ret;
		ret = -1;
		s = client->zsize(zset, &ret);
		assert(s.ok() && (ret != -1));
		s = client->zincr(zset, key, 3, &ret);
		assert(s.ok() && (ret == 3));
		s = client->zincr(zset, key, -1, &ret);
		assert(s.ok() && (ret == 2));
		
		client->zset(zset, "a", -1);
		client->zset(zset, "b", 3);
		client->zset(zset, "c", 4);
		int64_t score_max = 90;
	
		list.clear();
		s = client->zkeys(zset, "", NULL, &score_max, 2, &list);
		assert(s.ok() && list.size() <= 2);
	
		list.clear();
		s = client->zrange(zset, 0, 2, &list);
		assert(s.ok() && list.size() <= 4);
		list.clear();
		s = client->zrrange(zset, 0, 2, &list);
		assert(s.ok() && list.size() <= 4);
	
		list.clear();
		s = client->zscan(zset, "", NULL, &score_max, 2, &list);
		assert(s.ok() && list.size() <= 4);
		for(int i=0; i<list.size(); i++){
			if(i%2 == 0){
				printf("%s=", list[i].c_str());
			}else{
				printf("%s, ", list[i].c_str());
			}
		}
		printf("\n");
		
		list.clear();
		s = client->zrscan(zset, "", &score_max, NULL, 2, &list);
		assert(s.ok() && list.size() <= 4);
		for(int i=0; i<list.size(); i++){
			if(i%2 == 0){
				printf("%s=", list[i].c_str());
			}else{
				printf("%s, ", list[i].c_str());
			}
		}
		printf("\n");

		int64_t rank = -1;
		client->zrank(zset, "b", &rank);
		assert(s.ok() && (rank != -1));
		client->zrrank(zset, "b", &rank);
		assert(s.ok() && (rank != -1));

		ret = -1;
		s = client->zclear(zset, &ret);
		assert(s.ok() && ret != -1);
	}
	
	delete client;
	return 0;
}
