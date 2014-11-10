/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef UTIL_FILE_H_
#define UTIL_FILE_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static
inline bool file_exists(const char *filename){
	struct stat st;
	return stat(filename, &st) == 0;
}

static
inline bool is_dir(const char *filename){
	struct stat st;
	if(stat(filename, &st) == -1){
		return false;
	}
	return (bool)S_ISDIR(st.st_mode);
}

static
inline bool is_file(const char *filename){
	struct stat st;
	if(stat(filename, &st) == -1){
		return false;
	}
	return (bool)S_ISREG(st.st_mode);
}

#endif
