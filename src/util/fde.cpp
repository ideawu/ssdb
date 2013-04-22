#include "fde.h"

struct Fdevent* Fdevents::get_fde(int fd){
	while((int)events.size() <= fd){
		struct Fdevent *fde = new Fdevent();
		fde->fd = events.size();
		fde->s_flags = FDEVENT_NONE;
		fde->data.num = 0;
		fde->data.ptr = NULL;
		events.push_back(fde);
	}
	return events[fd];
}


#ifdef HAVE_EPOLL
#include "fde_epoll.cpp"
#else
#include "fde_select.cpp"
#endif
