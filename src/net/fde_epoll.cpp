/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef UTIL_FDE_EPOLL_H
#define UTIL_FDE_EPOLL_H

Fdevents::Fdevents(){
	ep_fd = epoll_create(1024);
}

Fdevents::~Fdevents(){
	for(int i=0; i<(int)events.size(); i++){
		delete events[i];
	}
	if(ep_fd){
		::close(ep_fd);
	}
	events.clear();
	ready_events.clear();
}

bool Fdevents::isset(int fd, int flag){
	struct Fdevent *fde = get_fde(fd);
	return (bool)(fde->s_flags & flag);
}

int Fdevents::set(int fd, int flags, int data_num, void *data_ptr){
	struct Fdevent *fde = get_fde(fd);
	if(fde->s_flags & flags){
		return 0;
	}
	int ctl_op = fde->s_flags? EPOLL_CTL_MOD : EPOLL_CTL_ADD;

	fde->s_flags |= flags;
	fde->data.num = data_num;
	fde->data.ptr = data_ptr;

	struct epoll_event epe;
	epe.data.ptr = fde;
	epe.events = 0;
	if(fde->s_flags & FDEVENT_IN)  epe.events |= EPOLLIN;
	if(fde->s_flags & FDEVENT_OUT) epe.events |= EPOLLOUT;

	int ret = epoll_ctl(ep_fd, ctl_op, fd, &epe);
	if(ret == -1){
		return -1;
	}
	return 0;
}

int Fdevents::del(int fd){
	struct epoll_event epe;
	int ret = epoll_ctl(ep_fd, EPOLL_CTL_DEL, fd, &epe);
	if(ret == -1){
		return -1;
	}

	struct Fdevent *fde = get_fde(fd);
	fde->s_flags = FDEVENT_NONE;
	return 0;
}

int Fdevents::clr(int fd, int flags){
	struct Fdevent *fde = get_fde(fd);
	if(!(fde->s_flags & flags)){
		return 0;
	}

	fde->s_flags &= ~flags;
	int ctl_op = fde->s_flags? EPOLL_CTL_MOD: EPOLL_CTL_DEL;

	struct epoll_event epe;
	epe.data.ptr = fde;
	epe.events = 0;
	if(fde->s_flags & FDEVENT_IN)  epe.events |= EPOLLIN;
	if(fde->s_flags & FDEVENT_OUT) epe.events |= EPOLLOUT;

	int ret = epoll_ctl(ep_fd, ctl_op, fd, &epe);
	if(ret == -1){
		return -1;
	}
	return 0;
}

const Fdevents::events_t* Fdevents::wait(int timeout_ms){
	struct Fdevent *fde;
	struct epoll_event *epe;
	ready_events.clear();

	int nfds = epoll_wait(ep_fd, ep_events, MAX_FDS, timeout_ms);
	if(nfds == -1){
		if(errno == EINTR){
			return &ready_events;
		}
		return NULL;
	}

	for(int i = 0; i < nfds; i++){
		epe = &ep_events[i];
		fde = (struct Fdevent *)epe->data.ptr;

		fde->events = FDEVENT_NONE;
		if(epe->events & EPOLLIN)  fde->events |= FDEVENT_IN;
		if(epe->events & EPOLLPRI) fde->events |= FDEVENT_IN;
		if(epe->events & EPOLLOUT) fde->events |= FDEVENT_OUT;
		if(epe->events & EPOLLHUP) fde->events |= FDEVENT_ERR;
		if(epe->events & EPOLLERR) fde->events |= FDEVENT_ERR;

		ready_events.push_back(fde);
	}
	return &ready_events;
}

#endif

