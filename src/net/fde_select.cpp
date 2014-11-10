/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef UTIL_FDE_SELECT_H
#define UTIL_FDE_SELECT_H

Fdevents::Fdevents(){
	maxfd = -1;
	FD_ZERO(&readset);
	FD_ZERO(&writeset);
}

Fdevents::~Fdevents(){
	for(size_t i=0; i<events.size(); i++){
		delete events[i];
	}
	events.clear();
	ready_events.clear();
}

bool Fdevents::isset(int fd, int flag){
	struct Fdevent *fde = get_fde(fd);
	return (bool)(fde->s_flags & flag);
}

int Fdevents::set(int fd, int flags, int data_num, void *data_ptr){
	if(fd > FD_SETSIZE - 1){
		return -1;
	}

	struct Fdevent *fde = get_fde(fd);
	if(fde->s_flags & flags){
		return 0;
	}

	if(flags & FDEVENT_IN)  FD_SET(fd, &readset);
	if(flags & FDEVENT_OUT) FD_SET(fd, &writeset);

	fde->data.num = data_num;
	fde->data.ptr = data_ptr;
	fde->s_flags |= flags;
	maxfd = fd > maxfd? fd: maxfd;

	return 0;
}

int Fdevents::del(int fd){
	FD_CLR(fd, &readset);
	FD_CLR(fd, &writeset);

	struct Fdevent *fde = get_fde(fd);
	fde->s_flags = FDEVENT_NONE;
	while(maxfd >= 0 && this->events[maxfd]->s_flags == 0){
		maxfd --;
	}
	return 0;
}

int Fdevents::clr(int fd, int flags){
	struct Fdevent *fde = get_fde(fd);
	if(!(fde->s_flags & flags)){
		return 0;
	}
	if(flags & FDEVENT_IN)  FD_CLR(fd, &readset);
	if(flags & FDEVENT_OUT) FD_CLR(fd, &writeset);

	fde->s_flags &= ~flags;
	while(this->events[maxfd]->s_flags == 0){
		maxfd --;
	}
	return 0;
}

const Fdevents::events_t* Fdevents::wait(int timeout_ms){
	struct timeval tv;
	struct Fdevent *fde;
	int i, ret;

	ready_events.clear();
	
	fd_set t_readset = readset;
	fd_set t_writeset = writeset;

	if(timeout_ms >= 0){
		tv.tv_sec =  timeout_ms / 1000;
		tv.tv_usec = (timeout_ms % 1000) * 1000;
		ret = ::select(maxfd + 1, &t_readset, &t_writeset, NULL, &tv);
	}else{
		ret = ::select(maxfd + 1, &t_readset, &t_writeset, NULL, NULL);
	}
	if(ret < 0){
		if(errno == EINTR){
			return &ready_events;
		}
		return NULL;
	}

	if(ret > 0){
		for(i = 0; i <= maxfd && (int)ready_events.size() < ret; i++){
			fde = this->events[i];

			fde->events = FDEVENT_NONE;
			if(FD_ISSET(i, &t_readset))  fde->events |= FDEVENT_IN;
			if(FD_ISSET(i, &t_writeset)) fde->events |= FDEVENT_OUT;

			if(fde->events){
				ready_events.push_back(fde);
			}
		}
	}

	return &ready_events;
}

#endif
