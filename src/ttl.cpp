#include <pthread.h>
#include <time.h>
#include "t_zset.h"
#include "ttl.h"

#define EXPIRATION_LIST_KEY "\xff\xff\xff\xff\xff|EXPIRE_LIST|KV"
#define NUM_EXPIRATION_KEYS_IN_MEM    1000

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

void ExpirationHandler::load_expiration_keys_from_db(int num){
	ZIterator *it;
	it = ssdb->zscan(this->list_name, "", "", "", num);
	while(it->next()){
		int64_t score = str_to_int64(it->score);
		if(score < 2000000000){
			// older version compatible
			score *= 1000;
		}
		expiration_keys.add(it->key, score);
	}
	delete it;
}

void ExpirationHandler::start(){
	this->load_expiration_keys_from_db(NUM_EXPIRATION_KEYS_IN_MEM);

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
	
	Locking l(&mutex);
	int ret = ssdb->zset(this->list_name, key, Bytes(data, size));
	if(ret == -1){
		return -1;
	}
	expiration_keys.add(key.String(), expired);
	if(expiration_keys.size() > NUM_EXPIRATION_KEYS_IN_MEM){
		expiration_keys.pop_back();
	}
	
	return 0;
}

void* ExpirationHandler::thread_func(void *arg){
	log_debug("ExpirationHandler started");
	ExpirationHandler *handler = (ExpirationHandler *)arg;
	
	while(!handler->thread_quit){
		SSDB *ssdb = handler->ssdb;
		if(!ssdb){
			break;
		}
		
		const std::string *key;
		int64_t score;
		{
			Locking l(&handler->mutex);
			if(handler->expiration_keys.front(&key, &score)){
				int64_t now = time_ms();
				if(score <= now){
					log_debug("expired %s", key->c_str());
					ssdb->del(*key);
					ssdb->zdel(handler->list_name, *key);
					handler->expiration_keys.pop_front();
					
					if(handler->expiration_keys.empty()){
						handler->load_expiration_keys_from_db(NUM_EXPIRATION_KEYS_IN_MEM);
					}
					continue;
				}
			}
		}
		usleep(50 * 1000);
	}
	
	log_debug("ExpirationHandler thread_func quit");
	handler->thread_quit = false;
	return (void *)NULL;
}
