/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include <pthread.h>
#include <time.h>
#include "../include.h"
#include "../util/log.h"
#include "ttl.h"

#define EXPIRATION_LIST_KEY "\xff\xff\xff\xff\xff|EXPIRE_LIST|KV"
#define BATCH_SIZE    1000

ExpirationHandler::ExpirationHandler(SSDB *ssdb){
	this->ssdb = ssdb;
	this->thread_quit = false;
	this->list_name = EXPIRATION_LIST_KEY;
	this->first_timeout = 0;
	this->start();
}

ExpirationHandler::~ExpirationHandler(){
	Locking l(&this->mutex);
	this->stop();
	ssdb = NULL;
}

void ExpirationHandler::start(){
	thread_quit = false;
	pthread_t tid;
	int err = pthread_create(&tid, NULL, &ExpirationHandler::thread_func, this);
	if(err != 0){
		log_fatal("can't create thread: %s", strerror(err));
		exit(0);
	}
}

void ExpirationHandler::stop(){
	thread_quit = true;
	for(int i=0; i<100; i++){
		if(!thread_quit){
			break;
		}
		usleep(10 * 1000);
	}
}

int ExpirationHandler::set_ttl(const Bytes &key, int64_t ttl){
	int64_t expired = time_ms() + ttl * 1000;
	char data[30];
	int size = snprintf(data, sizeof(data), "%" PRId64, expired);
	if(size <= 0){
		log_error("snprintf return error!");
		return -1;
	}

	int ret = ssdb->zset(this->list_name, key, Bytes(data, size));
	if(ret == -1){
		return -1;
	}
	if(expired < first_timeout){
		first_timeout = expired;
	}
	std::string s_key = key.String();
	if(!fast_keys.empty() && expired <= fast_keys.max_score()){
		fast_keys.add(s_key, expired);
		if(fast_keys.size() > BATCH_SIZE){
			log_debug("pop_back");
			fast_keys.pop_back();
		}
	}else{
		fast_keys.del(s_key);
		//log_debug("don't put in fast_keys");
	}
	
	return 0;
}

int ExpirationHandler::del_ttl(const Bytes &key){
	if(!this->fast_keys.empty()){
		fast_keys.del(key.String());
		ssdb->zdel(this->list_name, key);
	}
	return 0;
}

int64_t ExpirationHandler::get_ttl(const Bytes &key){
	std::string score;
	if(ssdb->zget(this->list_name, key, &score) == 1){
		int64_t ex = str_to_int64(score);
		return (ex - time_ms())/1000;
	}
	return -1;
}

void ExpirationHandler::load_expiration_keys_from_db(int num){
	ZIterator *it;
	it = ssdb->zscan(this->list_name, "", "", "", num);
	int n = 0;
	while(it->next()){
		n ++;
		std::string &key = it->key;
		int64_t score = str_to_int64(it->score);
		if(score < 2000000000){
			// older version compatible
			score *= 1000;
		}
		fast_keys.add(key, score);
	}
	delete it;
	log_debug("load %d keys into fast_keys", n);
}

void ExpirationHandler::expire_loop(){
	Locking l(&this->mutex);
	if(!this->ssdb){
		return;
	}

	if(this->fast_keys.empty()){
		this->load_expiration_keys_from_db(BATCH_SIZE);
		if(this->fast_keys.empty()){
			this->first_timeout = INT64_MAX;
			return;
		}
	}
	
	int64_t score;
	std::string key;
	if(this->fast_keys.front(&key, &score)){
		this->first_timeout = score;
		
		if(score <= time_ms()){
			log_debug("expired %s", key.c_str());
			ssdb->del(key);
			ssdb->zdel(this->list_name, key);
			this->fast_keys.pop_front();
		}
	}
}

void* ExpirationHandler::thread_func(void *arg){
	ExpirationHandler *handler = (ExpirationHandler *)arg;
	
	while(!handler->thread_quit){
		if(handler->first_timeout > time_ms()){
			usleep(10 * 1000);
			continue;
		}
		handler->expire_loop();
	}
	
	log_debug("ExpirationHandler thread quit");
	handler->thread_quit = false;
	return (void *)NULL;
}
