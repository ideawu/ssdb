/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef UTIL_DAEMON_H
#define UTIL_DAEMON_H

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
