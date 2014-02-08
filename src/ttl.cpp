#include <pthread.h>
#include <time.h>
#include "t_zset.h"
#include "ttl.h"

#define EXPIRATION_LIST_KEY "\xff\xff\xff\xff\xff|EXPIRE_LIST|KV"

ExpirationHandler::ExpirationHandler(SSDB *ssdb){
	this->ssdb = ssdb;
	this->thread_quit = false;
	this->list_name = EXPIRATION_LIST_KEY;
	this->start();
}

ExpirationHandler::~ExpirationHandler(){
	this->stop();
	ssdb = NULL;
	thread_quit = true;
}

void ExpirationHandler::start(){
	log_debug("loading expire_list");
	// TODO:
	
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

int ExpirationHandler::set_ttl(const Bytes &key, int ttl){
	time_t now = time(NULL) + ttl;
	char data[30];
	int size = snprintf(data, sizeof(data), "%ld", now);
	if(size <= 0){
		log_error("snprintf return error!");
		return -1;
	}
	return ssdb->zset(this->list_name, key, Bytes(data, size));
}

void* ExpirationHandler::thread_func(void *arg){
	log_debug("ExpirationHandler started");
	ExpirationHandler *handler = (ExpirationHandler *)arg;
	
	int last_expired = 0;
	int batch = 1000;
	while(!handler->thread_quit){
		last_expired = 0;
		
		time_t now = time(NULL);
		char data[30];
		int size = snprintf(data, sizeof(data), "%ld", now);
		if(size <= 0){
			log_error("snprintf return error!");
			continue;
		}
		
		SSDB *ssdb = handler->ssdb;
		if(!ssdb){
			break;
		}
		ZIterator *it;
		it = ssdb->zscan(handler->list_name, "", "", Bytes(data, size), batch);
		while(it->next()){
			log_trace("%s expired", it->key.c_str());
			last_expired ++;
			ssdb->del(it->key);
			ssdb->zdel(handler->list_name, it->key);
		}
		delete it;
		
		if(last_expired){
			log_trace("%d key(s) expired", last_expired);
			usleep(500 * 1000);
		}else{
			usleep(1000 * 1000);
		}
	}
	
	log_debug("ExpirationHandler thread_func quit");
	handler->thread_quit = false;
	return (void *)NULL;
}
