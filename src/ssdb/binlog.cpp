/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "binlog.h"
#include "const.h"
#include "../include.h"
#include "../util/log.h"
#include "../util/string_util.h"
#include <map>

/* Binlog */

Binlog::Binlog(uint64_t seq, char type, char cmd, const leveldb::Slice &key){
	buf.append((char *)(&seq), sizeof(uint64_t));
	buf.push_back(type);
	buf.push_back(cmd);
	buf.append(key.data(), key.size());
}

uint64_t Binlog::seq() const{
	return *((uint64_t *)(buf.data()));
}

char Binlog::type() const{
	return buf[sizeof(uint64_t)];
}

char Binlog::cmd() const{
	return buf[sizeof(uint64_t) + 1];
}

const Bytes Binlog::key() const{
	return Bytes(buf.data() + HEADER_LEN, buf.size() - HEADER_LEN);
}

int Binlog::load(const Bytes &s){
	if(s.size() < HEADER_LEN){
		return -1;
	}
	buf.assign(s.data(), s.size());
	return 0;
}

int Binlog::load(const leveldb::Slice &s){
	if(s.size() < HEADER_LEN){
		return -1;
	}
	buf.assign(s.data(), s.size());
	return 0;
}

int Binlog::load(const std::string &s){
	if(s.size() < HEADER_LEN){
		return -1;
	}
	buf.assign(s.data(), s.size());
	return 0;
}

std::string Binlog::dumps() const{
	std::string str;
	if(buf.size() < HEADER_LEN){
		return str;
	}
	str.reserve(128);

	char buf[20];
	snprintf(buf, sizeof(buf), "%" PRIu64 " ", this->seq());
	str.append(buf);

	switch(this->type()){
		case BinlogType::NOOP:
			str.append("noop ");
			break;
		case BinlogType::SYNC:
			str.append("sync ");
			break;
		case BinlogType::MIRROR:
			str.append("mirror ");
			break;
		case BinlogType::COPY:
			str.append("copy ");
			break;
		case BinlogType::CTRL:
			str.append("control ");
			break;
	}
	switch(this->cmd()){
		case BinlogCommand::NONE:
			str.append("none ");
			break;
		case BinlogCommand::KSET:
			str.append("set ");
			break;
		case BinlogCommand::KDEL:
			str.append("del ");
			break;
		case BinlogCommand::HSET:
			str.append("hset ");
			break;
		case BinlogCommand::HDEL:
			str.append("hdel ");
			break;
		case BinlogCommand::ZSET:
			str.append("zset ");
			break;
		case BinlogCommand::ZDEL:
			str.append("zdel ");
			break;
		case BinlogCommand::BEGIN:
			str.append("begin ");
			break;
		case BinlogCommand::END:
			str.append("end ");
			break;
		case BinlogCommand::QPUSH_BACK:
			str.append("qpush_back ");
			break;
		case BinlogCommand::QPUSH_FRONT:
			str.append("qpush_front ");
			break;
		case BinlogCommand::QPOP_BACK:
			str.append("qpop_back ");
			break;
		case BinlogCommand::QPOP_FRONT:
			str.append("qpop_front ");
			break;
		case BinlogCommand::QSET:
			str.append("qset ");
			break;
	}
	Bytes b = this->key();
	str.append(hexmem(b.data(), b.size()));
	return str;
}


/* SyncLogQueue */

static inline std::string encode_seq_key(uint64_t seq){
	seq = big_endian(seq);
	std::string ret;
	ret.push_back(DataType::SYNCLOG);
	ret.append((char *)&seq, sizeof(seq));
	return ret;
}

static inline uint64_t decode_seq_key(const leveldb::Slice &key){
	uint64_t seq = 0;
	if(key.size() == (sizeof(uint64_t) + 1) && key.data()[0] == DataType::SYNCLOG){
		seq = *((uint64_t *)(key.data() + 1));
		seq = big_endian(seq);
	}
	return seq;
}

BinlogQueue::BinlogQueue(leveldb::DB *db, bool enabled, int capacity){
	this->db = db;
	this->min_seq_ = 0;
	this->last_seq = 0;
	this->tran_seq = 0;
	this->capacity = capacity;
	this->enabled = enabled;

	if(!this->enabled){
		return;
	}
	
	Binlog log;
	if(this->find_last(&log) == 1){
		this->last_seq = log.seq();
	}
	if(this->find_min(&log) == 1){
		this->min_seq_ = log.seq();
	}
	log_info("binlogs capacity: %d, min: %" PRIu64 ", max: %" PRIu64 "",
		this->capacity, this->min_seq_, this->last_seq);
	// 这个方法有性能问题
	// 但是, 如果不执行清理, 如果将 capacity 修改大, 可能会导致主从同步问题
	//this->clean_obsolete_binlogs();

	// start cleaning thread
	thread_quit = false;
	pthread_t tid;
	int err = pthread_create(&tid, NULL, &BinlogQueue::log_clean_thread_func, this);
	if(err != 0){
		log_fatal("can't create thread: %s", strerror(err));
		exit(0);
	}
}

BinlogQueue::~BinlogQueue(){
	if(this->enabled){
		thread_quit = true;
		for(int i=0; i<100; i++){
			if(thread_quit == false){
				break;
			}
			usleep(10 * 1000);
		}
	}
	Locking l(&this->mutex);
	db = NULL;
}

std::string BinlogQueue::stats() const{
	std::string s;
	s.append("    capacity : " + str(capacity) + "\n");
	s.append("    min_seq  : " + str(min_seq_) + "\n");
	s.append("    max_seq  : " + str(last_seq) + "");
	return s;
}

void BinlogQueue::begin(){
	tran_seq = last_seq;
	batch.Clear();
}

void BinlogQueue::rollback(){
	tran_seq = 0;
}

leveldb::Status BinlogQueue::commit(){
	leveldb::WriteOptions write_opts;
	leveldb::Status s = db->Write(write_opts, &batch);
	if(s.ok()){
		last_seq = tran_seq;
		tran_seq = 0;
	}
	return s;
}

void BinlogQueue::add_log(char type, char cmd, const leveldb::Slice &key){
	if(!enabled){
		return;
	}
	tran_seq ++;
	Binlog log(tran_seq, type, cmd, key);
	batch.Put(encode_seq_key(tran_seq), log.repr());
}

void BinlogQueue::add_log(char type, char cmd, const std::string &key){
	if(!enabled){
		return;
	}
	leveldb::Slice s(key);
	this->add_log(type, cmd, s);
}

// leveldb put
void BinlogQueue::Put(const leveldb::Slice& key, const leveldb::Slice& value){
	batch.Put(key, value);
}

// leveldb delete
void BinlogQueue::Delete(const leveldb::Slice& key){
	batch.Delete(key);
}
	
int BinlogQueue::find_next(uint64_t next_seq, Binlog *log) const{
	if(this->get(next_seq, log) == 1){
		return 1;
	}
	uint64_t ret = 0;
	std::string key_str = encode_seq_key(next_seq);
	leveldb::ReadOptions iterate_options;
	leveldb::Iterator *it = db->NewIterator(iterate_options);
	it->Seek(key_str);
	if(it->Valid()){
		leveldb::Slice key = it->key();
		if(decode_seq_key(key) != 0){
			leveldb::Slice val = it->value();
			if(log->load(val) == -1){
				ret = -1;
			}else{
				ret = 1;
			}
		}
	}
	delete it;
	return ret;
}

int BinlogQueue::find_min(Binlog *log) const{
	int ret = 0;
	std::string key_str = encode_seq_key(0);
	leveldb::ReadOptions iterate_options;
	leveldb::Iterator *it = db->NewIterator(iterate_options);
	it->Seek(key_str);
	if(it->Valid()){
		leveldb::Slice key = it->key();
		if(decode_seq_key(key) != 0){
			leveldb::Slice val = it->value();
			if(log->load(val) == -1){
				ret = -1;
			}else{
				ret = 1;
			}
		}
	}
	delete it;
	return ret;
}

int BinlogQueue::find_last(Binlog *log) const{
	// 二分查找比 Iterator 快！
	{
		if(this->find_min(log) != 1){
			return 0;
		}
		// log_debug("min = %" PRIu64 "", log->seq());

		uint64_t begin = log->seq();
		uint64_t end = UINT64_MAX;
		while(begin != end){
			uint64_t curr = begin + (end - begin)/2;
			if(this->get(curr, log) == 0){
				// log_debug("[%" PRIu64 ", %" PRIu64 "] %" PRIu64 " not found", begin, end, curr);
				end = curr;
			}else{
				// log_debug("[%" PRIu64 ", %" PRIu64 "] %" PRIu64 " found", begin, end, curr);
				begin = curr + 1;
			}
		}
		
		end -= 1; // end 总是指向找不到的元素
		this->get(end, log);
		// log_debug("max = %" PRIu64 "", end);
		return 1;
	}
	
	// int ret = 0;
	// std::string key_str = encode_seq_key(UINT64_MAX);
	// leveldb::ReadOptions iterate_options;
	// leveldb::Iterator *it = db->NewIterator(iterate_options);
	// it->Seek(key_str);
	// if(!it->Valid()){
	// 	// Iterator::prev requires Valid, so we seek to last
	// 	it->SeekToLast();
	// }else{
	// 	// UINT64_MAX is not used
	// 	it->Prev();
	// }
	// if(it->Valid()){
	// 	leveldb::Slice key = it->key();
	// 	if(decode_seq_key(key) != 0){
	// 		leveldb::Slice val = it->value();
	// 		if(log->load(val) == -1){
	// 			ret = -1;
	// 		}else{
	// 			ret = 1;
	// 		}
	// 	}
	// }
	// delete it;
	// return ret;
}

int BinlogQueue::get(uint64_t seq, Binlog *log) const{
	std::string val;
	leveldb::Status s = db->Get(leveldb::ReadOptions(), encode_seq_key(seq), &val);
	if(s.ok()){
		if(log->load(val) != -1){
			return 1;
		}
	}
	return 0;
}

int BinlogQueue::update(uint64_t seq, char type, char cmd, const std::string &key){
	Binlog log(seq, type, cmd, key);
	leveldb::Status s = db->Put(leveldb::WriteOptions(), encode_seq_key(seq), log.repr());
	if(s.ok()){
		return 0;
	}
	return -1;
}

int BinlogQueue::del(uint64_t seq){
	leveldb::Status s = db->Delete(leveldb::WriteOptions(), encode_seq_key(seq));
	if(!s.ok()){
		return -1;
	}
	return 0;
}

void BinlogQueue::flush(){
	del_range(this->min_seq_, this->last_seq);
}

int BinlogQueue::del_range(uint64_t start, uint64_t end){
	while(start <= end){
		leveldb::WriteBatch batch;
		for(int count = 0; start <= end && count < 1000; start++, count++){
			batch.Delete(encode_seq_key(start));
		}
		
		Locking l(&this->mutex);
		if(!this->db){
			return -1;
		}
		leveldb::Status s = this->db->Write(leveldb::WriteOptions(), &batch);
		if(!s.ok()){
			return -1;
		}
	}
	return 0;
}

void* BinlogQueue::log_clean_thread_func(void *arg){
	BinlogQueue *logs = (BinlogQueue *)arg;
	
	while(!logs->thread_quit){
		if(!logs->db){
			break;
		}
		assert(logs->last_seq >= logs->min_seq_);

		if(logs->last_seq - logs->min_seq_ < logs->capacity + 10000){
			usleep(50 * 1000);
			continue;
		}
		
		uint64_t start = logs->min_seq_;
		uint64_t end = logs->last_seq - logs->capacity;
		logs->del_range(start, end);
		logs->min_seq_ = end + 1;
		log_info("clean %d logs[%" PRIu64 " ~ %" PRIu64 "], %d left, max: %" PRIu64 "",
			end-start+1, start, end, logs->last_seq - logs->min_seq_ + 1, logs->last_seq);
	}
	log_debug("binlog clean_thread quit");
	
	logs->thread_quit = false;
	return (void *)NULL;
}

// 因为老版本可能产生了断续的binlog
// 例如, binlog-1 存在, 但后面的被删除了, 然后到 binlog-100000 时又开始存在.
void BinlogQueue::clean_obsolete_binlogs(){
	std::string key_str = encode_seq_key(this->min_seq_);
	leveldb::ReadOptions iterate_options;
	leveldb::Iterator *it = db->NewIterator(iterate_options);
	it->Seek(key_str);
	if(it->Valid()){
		it->Prev();
	}
	uint64_t count = 0;
	while(it->Valid()){
		leveldb::Slice key = it->key();
		uint64_t seq = decode_seq_key(key);
		if(seq == 0){
			break;
		}
		this->del(seq);
		
		it->Prev();
		count ++;
	}
	delete it;
	if(count > 0){
		log_info("clean_obsolete_binlogs: %" PRIu64, count);
	}
}

// TESTING, slow, so not used
void BinlogQueue::merge(){
	std::map<std::string, uint64_t> key_map;
	uint64_t start = min_seq_;
	uint64_t end = last_seq;
	int reduce_count = 0;
	int total = 0;
	total = end - start + 1;
	(void)total; // suppresses warning
	log_trace("merge begin");
	for(; start <= end; start++){
		Binlog log;
		if(this->get(start, &log) == 1){
			if(log.type() == BinlogType::NOOP){
				continue;
			}
			std::string key = log.key().String();
			std::map<std::string, uint64_t>::iterator it = key_map.find(key);
			if(it != key_map.end()){
				uint64_t seq = it->second;
				this->update(seq, BinlogType::NOOP, BinlogCommand::NONE, "");
				//log_trace("merge update %" PRIu64 " to NOOP", seq);
				reduce_count ++;
			}
			key_map[key] = log.seq();
		}
	}
	log_trace("merge reduce %d of %d binlogs", reduce_count, total);
}
