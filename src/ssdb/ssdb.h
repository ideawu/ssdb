/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef SSDB_H_
#define SSDB_H_

#include <vector>
#include <string>
#include "const.h"
#include "options.h"
#include "iterator.h"

class Bytes;
class Config;

class SSDB{
public:
	SSDB(){}
	virtual ~SSDB(){};
	static SSDB* open(const Options &opt, const std::string &base_dir);
	
	virtual int flushdb() = 0;

	// return (start, end], not include start
	virtual Iterator* iterator(const std::string &start, const std::string &end, uint64_t limit) = 0;
	virtual Iterator* rev_iterator(const std::string &start, const std::string &end, uint64_t limit) = 0;

	//void flushdb();
	virtual uint64_t size() = 0;
	virtual std::vector<std::string> info() = 0;
	virtual void compact() = 0;
	virtual int key_range(std::vector<std::string> *keys) = 0;

	/* raw operates */

	// repl: whether to sync this operation to slaves
	virtual int raw_set(const Bytes &key, const Bytes &val) = 0;
	virtual int raw_del(const Bytes &key) = 0;
	virtual int raw_get(const Bytes &key, std::string *val) = 0;

	/* key value */

	virtual int set(const Bytes &key, const Bytes &val, char log_type=BinlogType::SYNC) = 0;
	virtual int setnx(const Bytes &key, const Bytes &val, char log_type=BinlogType::SYNC) = 0;
	virtual int del(const Bytes &key, char log_type=BinlogType::SYNC) = 0;
	// -1: error, 1: ok, 0: value is not an integer or out of range
	virtual int incr(const Bytes &key, int64_t by, int64_t *new_val, char log_type=BinlogType::SYNC) = 0;
	virtual int multi_set(const std::vector<Bytes> &kvs, int offset=0, char log_type=BinlogType::SYNC) = 0;
	virtual int multi_del(const std::vector<Bytes> &keys, int offset=0, char log_type=BinlogType::SYNC) = 0;
	virtual int setbit(const Bytes &key, int bitoffset, int on, char log_type=BinlogType::SYNC) = 0;
	virtual int getbit(const Bytes &key, int bitoffset) = 0;
	
	virtual int get(const Bytes &key, std::string *val) = 0;
	virtual int getset(const Bytes &key, std::string *val, const Bytes &newval, char log_type=BinlogType::SYNC) = 0;
	// return (start, end]
	virtual KIterator* scan(const Bytes &start, const Bytes &end, uint64_t limit) = 0;
	virtual KIterator* rscan(const Bytes &start, const Bytes &end, uint64_t limit) = 0;

	/* hash */

	virtual int hset(const Bytes &name, const Bytes &key, const Bytes &val, char log_type=BinlogType::SYNC) = 0;
	virtual int hdel(const Bytes &name, const Bytes &key, char log_type=BinlogType::SYNC) = 0;
	// -1: error, 1: ok, 0: value is not an integer or out of range
	virtual int hincr(const Bytes &name, const Bytes &key, int64_t by, int64_t *new_val, char log_type=BinlogType::SYNC) = 0;

	virtual int64_t hsize(const Bytes &name) = 0;
	virtual int64_t hclear(const Bytes &name) = 0;
	virtual int hget(const Bytes &name, const Bytes &key, std::string *val) = 0;
	virtual int hlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
			std::vector<std::string> *list) = 0;
	virtual int hrlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
			std::vector<std::string> *list) = 0;
	virtual HIterator* hscan(const Bytes &name, const Bytes &start, const Bytes &end, uint64_t limit) = 0;
	virtual HIterator* hrscan(const Bytes &name, const Bytes &start, const Bytes &end, uint64_t limit) = 0;

	/* zset */

	virtual int zset(const Bytes &name, const Bytes &key, const Bytes &score, char log_type=BinlogType::SYNC) = 0;
	virtual int zdel(const Bytes &name, const Bytes &key, char log_type=BinlogType::SYNC) = 0;
	// -1: error, 1: ok, 0: value is not an integer or out of range
	virtual int zincr(const Bytes &name, const Bytes &key, int64_t by, int64_t *new_val, char log_type=BinlogType::SYNC) = 0;
	
	virtual int64_t zsize(const Bytes &name) = 0;
	/**
	 * @return -1: error; 0: not found; 1: found
	 */
	virtual int zget(const Bytes &name, const Bytes &key, std::string *score) = 0;
	virtual int64_t zrank(const Bytes &name, const Bytes &key) = 0;
	virtual int64_t zrrank(const Bytes &name, const Bytes &key) = 0;
	virtual ZIterator* zrange(const Bytes &name, uint64_t offset, uint64_t limit) = 0;
	virtual ZIterator* zrrange(const Bytes &name, uint64_t offset, uint64_t limit) = 0;
	/**
	 * scan by score, but won't return @key if key.score=score_start.
	 * return (score_start, score_end]
	 */
	virtual ZIterator* zscan(const Bytes &name, const Bytes &key,
			const Bytes &score_start, const Bytes &score_end, uint64_t limit) = 0;
	virtual ZIterator* zrscan(const Bytes &name, const Bytes &key,
			const Bytes &score_start, const Bytes &score_end, uint64_t limit) = 0;
	virtual int zlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
			std::vector<std::string> *list) = 0;
	virtual int zrlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
			std::vector<std::string> *list) = 0;
	
	virtual int64_t qsize(const Bytes &name) = 0;
	// @return 0: empty queue, 1: item peeked, -1: error
	virtual int qfront(const Bytes &name, std::string *item) = 0;
	// @return 0: empty queue, 1: item peeked, -1: error
	virtual int qback(const Bytes &name, std::string *item) = 0;
	// @return -1: error, other: the new length of the queue
	virtual int64_t qpush_front(const Bytes &name, const Bytes &item, char log_type=BinlogType::SYNC) = 0;
	virtual int64_t qpush_back(const Bytes &name, const Bytes &item, char log_type=BinlogType::SYNC) = 0;
	// @return 0: empty queue, 1: item popped, -1: error
	virtual int qpop_front(const Bytes &name, std::string *item, char log_type=BinlogType::SYNC) = 0;
	virtual int qpop_back(const Bytes &name, std::string *item, char log_type=BinlogType::SYNC) = 0;
	virtual int qfix(const Bytes &name) = 0;
	virtual int qlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
			std::vector<std::string> *list) = 0;
	virtual int qrlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
			std::vector<std::string> *list) = 0;
	virtual int qslice(const Bytes &name, int64_t offset, int64_t limit,
			std::vector<std::string> *list) = 0;
	virtual int qget(const Bytes &name, int64_t index, std::string *item) = 0;
	virtual int qset(const Bytes &name, int64_t index, const Bytes &item, char log_type=BinlogType::SYNC) = 0;
	virtual int qset_by_seq(const Bytes &name, uint64_t seq, const Bytes &item, char log_type=BinlogType::SYNC) = 0;
};


#endif
