#ifndef SSDB_H_
#define SSDB_H_

#include "include.h"
#include <vector>
#include "leveldb/db.h"
#include "leveldb/options.h"

#include "util/log.h"
#include "util/bytes.h"
#include "util/config.h"
#include "util/thread.h"
#include "iterator.h"
#include "binlog.h"

class KIterator;
class HIterator;
class ZIterator;
//class Slave;


class SSDB{
private:
	leveldb::DB* db;
	leveldb::Options options;
	int sync_speed_;
	
	std::string kv_range_s;
	std::string kv_range_e;
	
	SSDB();
public:
	leveldb::DB* meta_db;
	BinlogQueue *binlogs;
	
	~SSDB();
	static SSDB* open(const Config &conf, const std::string &base_dir);

	// return (start, end], not include start
	Iterator* iterator(const std::string &start, const std::string &end, uint64_t limit) const;
	Iterator* rev_iterator(const std::string &start, const std::string &end, uint64_t limit) const;

	//void flushdb();
	std::vector<std::string> info() const;
	void compact() const;
	int key_range(std::vector<std::string> *keys) const;
	int sync_speed() const { return sync_speed_; }
	
	int set_kv_range(const std::string &s, const std::string &e);
	int get_kv_range(std::string *s, std::string *e);
	bool in_kv_range(const std::string &key) const;

	/* raw operates */

	// repl: whether to sync this operation to slaves
	int raw_set(const Bytes &key, const Bytes &val) const;
	int raw_del(const Bytes &key) const;
	int raw_get(const Bytes &key, std::string *val) const;

	/* key value */

	int set(const Bytes &key, const Bytes &val, char log_type=BinlogType::SYNC);
	int setnx(const Bytes &key, const Bytes &val, char log_type=BinlogType::SYNC);
	int del(const Bytes &key, char log_type=BinlogType::SYNC);
	// -1: error, 1: ok, 0: value is not an integer or out of range
	int incr(const Bytes &key, int64_t by, int64_t *new_val, char log_type=BinlogType::SYNC);
	int multi_set(const std::vector<Bytes> &kvs, int offset=0, char log_type=BinlogType::SYNC);
	int multi_del(const std::vector<Bytes> &keys, int offset=0, char log_type=BinlogType::SYNC);
	int setbit(const Bytes &key, int bitoffset, int on, char log_type=BinlogType::SYNC);
	int getbit(const Bytes &key, int bitoffset);
	
	int get(const Bytes &key, std::string *val) const;
	int getset(const Bytes &key, std::string *val, const Bytes &newval, char log_type=BinlogType::SYNC);
	// return (start, end]
	KIterator* scan(const Bytes &start, const Bytes &end, uint64_t limit) const;
	KIterator* rscan(const Bytes &start, const Bytes &end, uint64_t limit) const;

	/* hash */

	int hset(const Bytes &name, const Bytes &key, const Bytes &val, char log_type=BinlogType::SYNC);
	int hdel(const Bytes &name, const Bytes &key, char log_type=BinlogType::SYNC);
	// -1: error, 1: ok, 0: value is not an integer or out of range
	int hincr(const Bytes &name, const Bytes &key, int64_t by, int64_t *new_val, char log_type=BinlogType::SYNC);
	//int multi_hset(const Bytes &name, const std::vector<Bytes> &kvs, int offset=0, char log_type=BinlogType::SYNC);
	//int multi_hdel(const Bytes &name, const std::vector<Bytes> &keys, int offset=0, char log_type=BinlogType::SYNC);

	int64_t hsize(const Bytes &name) const;
	int hget(const Bytes &name, const Bytes &key, std::string *val) const;
	int hlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
			std::vector<std::string> *list) const;
	int hrlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
			std::vector<std::string> *list) const;
	HIterator* hscan(const Bytes &name, const Bytes &start, const Bytes &end, uint64_t limit) const;
	HIterator* hrscan(const Bytes &name, const Bytes &start, const Bytes &end, uint64_t limit) const;

	/* zset */

	int zset(const Bytes &name, const Bytes &key, const Bytes &score, char log_type=BinlogType::SYNC);
	int zdel(const Bytes &name, const Bytes &key, char log_type=BinlogType::SYNC);
	// -1: error, 1: ok, 0: value is not an integer or out of range
	int zincr(const Bytes &name, const Bytes &key, int64_t by, int64_t *new_val, char log_type=BinlogType::SYNC);
	//int multi_zset(const Bytes &name, const std::vector<Bytes> &kvs, int offset=0, char log_type=BinlogType::SYNC);
	//int multi_zdel(const Bytes &name, const std::vector<Bytes> &keys, int offset=0, char log_type=BinlogType::SYNC);
	
	int64_t zsize(const Bytes &name) const;
	/**
	 * @return -1: error; 0: not found; 1: found
	 */
	int zget(const Bytes &name, const Bytes &key, std::string *score) const;
	int64_t zrank(const Bytes &name, const Bytes &key) const;
	int64_t zrrank(const Bytes &name, const Bytes &key) const;
	ZIterator* zrange(const Bytes &name, uint64_t offset, uint64_t limit);
	ZIterator* zrrange(const Bytes &name, uint64_t offset, uint64_t limit);
	/**
	 * scan by score, but won't return @key if key.score=score_start.
	 * return (score_start, score_end]
	 */
	ZIterator* zscan(const Bytes &name, const Bytes &key,
			const Bytes &score_start, const Bytes &score_end, uint64_t limit) const;
	ZIterator* zrscan(const Bytes &name, const Bytes &key,
			const Bytes &score_start, const Bytes &score_end, uint64_t limit) const;
	int zlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
			std::vector<std::string> *list) const;
	int zrlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
			std::vector<std::string> *list) const;
	
	int64_t qsize(const Bytes &name);
	// @return 0: empty queue, 1: item peeked, -1: error
	int qfront(const Bytes &name, std::string *item);
	// @return 0: empty queue, 1: item peeked, -1: error
	int qback(const Bytes &name, std::string *item);
	// @return -1: error, other: the new length of the queue
	int64_t qpush_front(const Bytes &name, const Bytes &item, char log_type=BinlogType::SYNC);
	int64_t qpush_back(const Bytes &name, const Bytes &item, char log_type=BinlogType::SYNC);
	// @return 0: empty queue, 1: item popped, -1: error
	int qpop_front(const Bytes &name, std::string *item, char log_type=BinlogType::SYNC);
	int qpop_back(const Bytes &name, std::string *item, char log_type=BinlogType::SYNC);
	int qfix(const Bytes &name);
	int qlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
			std::vector<std::string> *list);
	int qrlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
			std::vector<std::string> *list);
	int qslice(const Bytes &name, int64_t offset, int64_t limit,
			std::vector<std::string> *list);
	int qget(const Bytes &name, int64_t index, std::string *item);
	int qset(const Bytes &name, int64_t index, const Bytes &item);

private:
	int64_t _qpush(const Bytes &name, const Bytes &item, uint64_t front_or_back_seq, char log_type=BinlogType::SYNC);
	int _qpop(const Bytes &name, std::string *item, uint64_t front_or_back_seq, char log_type=BinlogType::SYNC);
};


#endif
