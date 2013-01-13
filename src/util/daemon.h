#ifndef UTIL_DAEMON_H
#define UTIL_DAEMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int daemonize(const char *dir=NULL){
	switch(fork()){
		case -1:
			return -1;
		case 0:
			break;
		default:
			exit(0);
	}
	if(setsid() == -1){
		exit(0);
	}
	if(dir != NULL){
		if(chdir(dir) == -1){
			exit(0);
		}
	}

	if(close(STDIN_FILENO) == -1){
		exit(0);
	}
	if(close(STDOUT_FILENO) == -1){
		exit(0);
	}
	if(close(STDERR_FILENO) == -1){
		exit(0);
	}

	int fd = open("/dev/null", O_RDWR, 0);
	if(fd == -1){
		exit(0);
	}
	if(dup2(fd, STDIN_FILENO) == -1){
		exit(0);
	}
	if(dup2(fd, STDOUT_FILENO) == -1){
		exit(0);
	}
	if(dup2(fd, STDERR_FILENO) == -1){
		exit(0);
	}

	return 0;
}

#endif
