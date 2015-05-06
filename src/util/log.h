/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef UTIL_LOG_H
#define UTIL_LOG_H

#include <inttypes.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <assert.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string>

class Logger{
	public:
		static const int LEVEL_NONE		= (-1);
		static const int LEVEL_MIN		= 0;
		static const int LEVEL_FATAL	= 0;
		static const int LEVEL_ERROR	= 1;
		static const int LEVEL_WARN		= 2;
		static const int LEVEL_INFO		= 3;
		static const int LEVEL_DEBUG	= 4;
		static const int LEVEL_TRACE	= 5;
		static const int LEVEL_MAX		= 5;

		static int get_level(const char *levelname);
		
		static Logger* shared();
		
		std::string level_name();
		std::string output_name();
		uint64_t rotate_size();
	private:
		FILE *fp;
		char filename[PATH_MAX];
		int level_;
		pthread_mutex_t *mutex;

		uint64_t rotate_size_;
		struct{
			uint64_t w_curr;
			uint64_t w_total;
		}stats;

		void rotate();
		void threadsafe();
	public:
		Logger();
		~Logger();

		int level(){
			return level_;
		}

		void set_level(int level){
			this->level_ = level;
		}

		int open(FILE *fp, int level=LEVEL_DEBUG, bool is_threadsafe=false);
		int open(const char *filename, int level=LEVEL_DEBUG,
			bool is_threadsafe=false, uint64_t rotate_size=0);
		void close();

		int logv(int level, const char *fmt, va_list ap);

		int trace(const char *fmt, ...);
		int debug(const char *fmt, ...);
		int info(const char *fmt, ...);
		int warn(const char *fmt, ...);
		int error(const char *fmt, ...);
		int fatal(const char *fmt, ...);
};


int log_open(FILE *fp, int level=Logger::LEVEL_DEBUG, bool is_threadsafe=false);
int log_open(const char *filename, int level=Logger::LEVEL_DEBUG,
	bool is_threadsafe=false, uint64_t rotate_size=0);
int log_level();
void set_log_level(int level);
void set_log_level(const char *s);
int log_write(int level, const char *fmt, ...);


#ifndef IOS
	#ifdef NDEBUG
		#define log_trace(fmt, args...) do{}while(0)
	#else
		#define log_trace(fmt, args...)	\
			log_write(Logger::LEVEL_TRACE, "%s(%d): " fmt, __FILE__, __LINE__, ##args)
	#endif

	#define log_debug(fmt, args...)	\
		log_write(Logger::LEVEL_DEBUG, "%s(%d): " fmt, __FILE__, __LINE__, ##args)
	#define log_info(fmt, args...)	\
		log_write(Logger::LEVEL_INFO,  "%s(%d): " fmt, __FILE__, __LINE__, ##args)
	#define log_warn(fmt, args...)	\
		log_write(Logger::LEVEL_WARN,  "%s(%d): " fmt, __FILE__, __LINE__, ##args)
	#define log_error(fmt, args...)	\
		log_write(Logger::LEVEL_ERROR, "%s(%d): " fmt, __FILE__, __LINE__, ##args)
	#define log_fatal(fmt, args...)	\
		log_write(Logger::LEVEL_FATAL, "%s(%d): " fmt, __FILE__, __LINE__, ##args)
#else
	#define log_trace(fmt, args...) do{}while(0)
	#define log_debug(fmt, args...) do{}while(0)
	#define log_info(fmt, args...) do{}while(0)
	#define log_warn(fmt, args...) do{}while(0)
	#define log_error(fmt, args...) do{}while(0)
	#define log_fatal(fmt, args...) do{}while(0)
#endif

#endif
