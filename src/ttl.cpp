#include <pthread.h>
#include <time.h>
#include "t_zset.h"
#include "ttl.h"

#define EXPIRATION_LIST_KEY "\xff\xff\xff\xff\xff|EXPIRE_LIST|KV"
#define BATCH_SIZE    100

ExpirationHandler::ExpirationHandler(SSDB *ssdb){
	this->ssdb = ssdb;
	this->thread_quit = false;
	this->list_name = EXPIRATION_LIST_KEY;
	first_timeout = 0;
	this->start();
}

ExpirationHandler::~ExpirationHandler(){
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
	
	return 0;
}

int ExpirationHandler::del_ttl(const Bytes &key){
	ssdb->zdel(this->list_name, key);
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

void* ExpirationHandler::thread_func(void *arg){
	log_debug("ExpirationHandler started");
	ExpirationHandler *handler = (ExpirationHandler *)arg;
	
	while(!handler->thread_quit){
		SSDB *ssdb = handler->ssdb;
		if(!ssdb){
			break;
		}
		int64_t now = time_ms();
		if(handler->first_timeout > now){
			usleep(10 * 1000);
			continue;
		}
		
		Locking l(&handler->mutex);
		ZIterator *it;
		it = ssdb->zscan(handler->list_name, "", "", "", BATCH_SIZE);
		while(it->next()){
			std::string &key = it->key;
			int64_t score = str_to_int64(it->score);
			if(score < 2000000000){
				// older version compatible
				score *= 1000;
			}
			handler->first_timeout = score;
			
			if(score > now){
				break;
			}
			log_debug("expired %s", key.c_str());
			ssdb->del(key);
			ssdb->zdel(handler->list_name, key);
		}
		delete it;
	}
	
	log_debug("ExpirationHandler thread_func quit");
	handler->thread_quit = false;
	return (void *)NULL;
}
