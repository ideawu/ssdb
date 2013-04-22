#ifndef SSDB_H_
#define SSDB_H_

#include "include.h"
#include <vector>
#include "leveldb/db.h"
#include "leveldb/options.h"

#include "util/log.h"
#include "util/bytes.h"
#include "util/config.h"

#include "iterator.h"
#include "repl.h"


static const int SSDB_SCORE_WIDTH		= 9;
static const int SSDB_KEY_LEN_MAX		= 200;


static inline double microtime(){
	struct timeval now;
	gettimeofday(&now, NULL);
	double ret = now.tv_sec + now.tv_usec/1000.0/1000.0;
	return ret;
}

class DataType{
public:
	static const int KV			= 'k';
	static const int ZSCORE		= 's';
	static const int ZSET		= 'z'; // sorted set(sorted by score)
	static const int ZSIZE		= 'Z';
	static const int HASH		= 'h'; // hashmap(sorted by key)
	static const int HSIZE		= 'H';
};

class KIterator;
class HIterator;
class ZIterator;
class Slave;

class SSDB{
private:
	leveldb::DB* db;
	leveldb::DB* meta_db;
	leveldb::Options options;

	Slave *slave;
	std::vector<Slave *> slaves;
	
	SSDB();
public:
	~SSDB();
	const MyReplication *replication;

	static SSDB* open(const Config &conf, const std::string &base_dir);

	// return (start, end)
	Iterator* iterator(const std::string &start, const std::string &end, int limit) const;
	Iterator* rev_iterator(const std::string &start, const std::string &end, int limit) const;

	std::vector<std::string> info() const;

	/* raw operates */

	// repl: whether to sync this operation to slaves
	int raw_set(const Bytes &key, const Bytes &val, bool is_mirror=false) const;
	int raw_del(const Bytes &key, bool is_mirror=false) const;
	int raw_get(const Bytes &key, std::string *val) const;

	/* key value */

	int set(const Bytes &key, const Bytes &val) const;
	int get(const Bytes &key, std::string *val) const;
	int del(const Bytes &key) const;
	int incr(const Bytes &key, int64_t by, std::string *new_val) const;
	// return (start, end)
	KIterator* scan(const Bytes &start, const Bytes &end, int limit) const;
	KIterator* rscan(const Bytes &start, const Bytes &end, int limit) const;
	int multi_set(const std::vector<Bytes> &kvs, int offset=0) const;
	int multi_del(const std::vector<Bytes> &keys, int offset=0) const;

	/* hash */

	int64_t hsize(const Bytes &name) const;
	int hset(const Bytes &name, const Bytes &key, const Bytes &val) const;
	int hget(const Bytes &name, const Bytes &key, std::string *val) const;
	int hdel(const Bytes &name, const Bytes &key) const;
	int hincr(const Bytes &name, const Bytes &key, int64_t by, std::string *new_val) const;
	HIterator* hscan(const Bytes &name, const Bytes &start, const Bytes &end, int limit) const;
	HIterator* hrscan(const Bytes &name, const Bytes &start, const Bytes &end, int limit) const;
	int hlist(const Bytes &name_s, const Bytes &name_e, int limit,
			std::vector<std::string> *list) const;
	int multi_hset(const Bytes &name, const std::vector<Bytes> &kvs, int offset=0) const;
	int multi_hdel(const Bytes &name, const std::vector<Bytes> &keys, int offset=0) const;

	/* zset */

	int64_t zsize(const Bytes &name) const;
	int zset(const Bytes &name, const Bytes &key, const Bytes &score) const;
	int zget(const Bytes &name, const Bytes &key, std::string *score) const;
	int zdel(const Bytes &name, const Bytes &key) const;
	int zincr(const Bytes &name, const Bytes &key, int64_t by, std::string *new_val) const;
	/**
	 * scan by score, but won't return @key if key.score=score_start.
	 * return (score_start, score_end)
	 */
	ZIterator* zscan(const Bytes &name, const Bytes &key,
			const Bytes &score_start, const Bytes &score_end, int limit) const;
	ZIterator* zrscan(const Bytes &name, const Bytes &key,
			const Bytes &score_start, const Bytes &score_end, int limit) const;
	int zlist(const Bytes &name_s, const Bytes &name_e, int limit,
			std::vector<std::string> *list) const;
	int multi_zset(const Bytes &name, const std::vector<Bytes> &kvs, int offset=0) const;
	int multi_zdel(const Bytes &name, const std::vector<Bytes> &keys, int offset=0) const;
};


#endif
