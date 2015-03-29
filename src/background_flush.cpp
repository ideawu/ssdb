/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include <pthread.h>
#include "background_flush.h"
#include "serv.h"
#include "util/log.h"

#if defined(WIN32) || defined(WIN64)

#include <windows.h>

void usleep(__int64 usec) 
{ 
    HANDLE timer; 
    LARGE_INTEGER ft; 

    ft.QuadPart = -(10*usec); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL); 
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
    WaitForSingleObject(timer, INFINITE); 
    CloseHandle(timer); 
}
#endif

BackgroundFlush::BackgroundFlush(SSDBServer *serv,const Config *config):_serv(serv),_conf(config)
{}

BackgroundFlush::~BackgroundFlush(){
    this->end();
	log_debug("BackgroundFlush finalized");
}

void BackgroundFlush::start(){
	struct run_arg *arg = new run_arg();
	arg->background = this;

	int err = pthread_create(&this->tid, NULL, &BackgroundFlush::_run_thread, arg);
	if(err != 0){
		log_error("can't create thread: %s", strerror(err));
	}
}

void BackgroundFlush::end(){
    
    int err=pthread_cancel(this->tid);
    if(err != 0){
		log_error("can't stop thread: %s", strerror(err));
    };

}

void* BackgroundFlush::_run_thread(void *arg){
    struct run_arg *p = (struct run_arg*)arg;
	const BackgroundFlush *background = p->background;
	delete p;

    int time_ms=1000;
    {
        int flush_period_ms=background->_conf->get_num("persistent.flush_period_ms");
        if(flush_period_ms<0){
            return (void *)NULL;
        }else{
            time_ms=flush_period_ms;
        }
    }

    while(1){
        pthread_testcancel();

        int fd=open(background->_serv->get_meta_dir(),O_RDONLY);
        if(fd<0){
            log_error("failed to open: %s",background->_serv->get_meta_dir());
        }else{
            if(fsync(fd)<0) log_error("failed to fsync: %s",background->_serv->get_meta_dir());
            close(fd);
        }
        
        fd=open(background->_serv->get_data_dir(),O_RDONLY);
        if(fd<0){
            log_error("failed to open: %s",background->_serv->get_data_dir());
        }else{
            if(fsync(fd)<0) log_error("failed to fsync: %s",background->_serv->get_data_dir());
            close(fd);
        }
        
        pthread_testcancel();
        
        usleep(1000*time_ms);
    }
	
    return (void *)NULL;
}
