/**domain socket from redis source code.**/

#include "ds_network.h"

int anetUnixServer(char *err, const char *path, mode_t perm)
{
	int s;
	struct sockaddr_un sa;
	if ((s = anetCreateSocket(err,AF_LOCAL)) == ANET_ERR)
		return ANET_ERR;
	memset(&sa,0,sizeof(sa));
	sa.sun_family = AF_LOCAL;
	strncpy(sa.sun_path,path,sizeof(sa.sun_path)-1);

	if (anetListen(err,s,(struct sockaddr*)&sa,sizeof(sa)) == ANET_ERR){
		return ANET_ERR;
	}

	if (perm)
		chmod(sa.sun_path, perm);
	return s;//返回sock
}

int anetUnixAccept(char *err, int s) {
	int fd;
	struct sockaddr_un sa;
	socklen_t salen = sizeof(sa);
	if ((fd = anetGenericAccept(err,s,(struct sockaddr*)&sa,&salen)) == ANET_ERR)
		return ANET_ERR;

	return fd;//返回accept的fd
}
//返回client socket
int anetUnixConnect(char *err, const char *path){
	return anetUnixGenericConnect(err,path,ANET_CONNECT_NONE);
}
//返回client socket
int anetUnixNonBlockConnect(char *err, char *path){
	return anetUnixGenericConnect(err,path,ANET_CONNECT_NONBLOCK);
}

int anetUnixGenericConnect(char *err, const char *path, int flags){
	int s;
	struct sockaddr_un sa;

	if ((s = anetCreateSocket(err,AF_LOCAL)) == ANET_ERR)
		return ANET_ERR;

	sa.sun_family = AF_LOCAL;
	strncpy(sa.sun_path,path,sizeof(sa.sun_path)-1);
	if (flags & ANET_CONNECT_NONBLOCK) {
		if (anetNonBlock(err,s) != ANET_OK)
			return ANET_ERR;
	}
	if (connect(s,(struct sockaddr*)&sa,sizeof(sa)) == -1) {
		if (errno == EINPROGRESS &&
			flags & ANET_CONNECT_NONBLOCK)
			return s;

		anetSetError(err, "connect: %s", strerror(errno));
		close(s);
		return ANET_ERR;
	}
	return s;
}

int anetGenericAccept(char *err, int s, struct sockaddr *sa, socklen_t *len) {
	int fd;
	while(1) {
		fd = accept(s,sa,len);
		if (fd == -1) {
			if (errno == EINTR)
				continue;
			else {
				anetSetError(err, "accept: %s", strerror(errno));
				return ANET_ERR;
			}
		}
		break;
	}
	return fd;
}

int anetListen(char *err, int s, struct sockaddr *sa, socklen_t len) {
	if (bind(s,sa,len) == -1) {
		anetSetError(err, "bind: %s", strerror(errno));
		close(s);
		return ANET_ERR;
	}

	/* Use a backlog of 512 entries. We pass 511 to the listen() call because
	* the kernel does: backlogsize = roundup_pow_of_two(backlogsize + 1);
	* which will thus give us a backlog of 512 entries */
	if (listen(s, 511) == -1) {
		anetSetError(err, "listen: %s", strerror(errno));
		close(s);
		return ANET_ERR;
	}
	return ANET_OK;
}

int anetCreateSocket(char *err, int domain) {
	int s, on = 1;
	if ((s = socket(domain, SOCK_STREAM, 0)) == -1) {
		anetSetError(err, "creating socket: %s", strerror(errno));
		return ANET_ERR;
	}

	/* Make sure connection-intensive things like the redis benckmark
	* will be able to close/open sockets a zillion of times */
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
		anetSetError(err, "setsockopt SO_REUSEADDR: %s", strerror(errno));
		return ANET_ERR;
	}
	return s;
}

int anetNonBlock(char *err, int fd){
	int flags;

	/* Set the socket nonblocking.
	* Note that fcntl(2) for F_GETFL and F_SETFL can't be
	* interrupted by a signal. */
	if ((flags = fcntl(fd, F_GETFL)) == -1) {
		anetSetError(err, "fcntl(F_GETFL): %s", strerror(errno));
		return ANET_ERR;
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		anetSetError(err, "fcntl(F_SETFL,O_NONBLOCK): %s", strerror(errno));
		return ANET_ERR;
	}
	return ANET_OK;
}

void anetSetError(char *err, const char *fmt, ...){
	va_list ap;

	if (!err) return;
	va_start(ap, fmt);
	vsnprintf(err, ANET_ERR_LEN, fmt, ap);
	va_end(ap);
}
