/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef SSDB_BINLOG_H_
#define SSDB_BINLOG_H_

#include <string>
#include "leveldb/db.h"
#include "leveldb/options.h"
#include "leveldb/slice.h"
#include "leveldb/status.h"
#include "leveldb/write_batch.h"
#include "../util/thread.h"
#include "../util/bytes.h"


class Binlog{
private:
	std::string buf;
	static const unsigned int HEADER_LEN = sizeof(uint64_t) + 2;
public:
	Binlog(){}
	Binlog(uint64_t seq, char type, char cmd, const leveldb::Slice &key);
		
	int load(const Bytes &s);
	int load(const leveldb::Slice &s);
	int load(const std::string &s);

	uint64_t seq() const;
	char type() const;
	char cmd() const;
	const Bytes key() const;

	const char* data() const{
		return buf.data();
	}
	int size() const{
		return (int)buf.size();
	}
	const std::string repr() const{
		return this->buf;
	}
	std::string dumps() const;
};

// circular queue
class BinlogQueue{
private:
	leveldb::DB *db;
	uint64_t min_seq_;
	uint64_t last_seq;
	uint64_t tran_seq;
	int capacity;
	leveldb::WriteBatch batch;

	volatile bool thread_quit;
	static void* log_clean_thread_func(void *arg);
	int del(uint64_t seq);
	// [start, end] includesive
	int del_range(uint64_t start, uint64_t end);
	
	void clean_obsolete_binlogs();
	void merge();
	bool enabled;
public:
	Mutex mutex;

	BinlogQueue(leveldb::DB *db, bool enabled=true, int capacity=20000000);
	~BinlogQueue();
	void begin();
	void rollback();
	leveldb::Status commit();
	// leveldb put
	void Put(const leveldb::Slice& key, const leveldb::Slice& value);
	// leveldb delete
	void Delete(const leveldb::Slice& key);
	void add_log(char type, char cmd, const leveldb::Slice &key);
	void add_log(char type, char cmd, const std::string &key);
		
	int get(uint64_t seq, Binlog *log) const;
	int update(uint64_t seq, char type, char cmd, const std::string &key);
		
	void flush();
		
	/** @returns
	 1 : log.seq greater than or equal to seq
	 0 : not found
	 -1: error
	 */
	int find_next(uint64_t seq, Binlog *log) const;
	int find_min(Binlog *log) const;
	int find_last(Binlog *log) const;
	
	uint64_t min_seq() const{
		return min_seq_;
	}
	uint64_t max_seq() const{
		return last_seq;
	}
		
	std::string stats() const;
};

class Transaction{
private:
	BinlogQueue *logs;
public:
	Transaction(BinlogQueue *logs){
		this->logs = logs;
		logs->mutex.lock();
		logs->begin();
	}
	
	~Transaction(){
		// it is safe to call rollback after commit
		logs->rollback();
		logs->mutex.unlock();
	}
};


#endif
