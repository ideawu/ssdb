#include "t_hash.h"
#include "ssdb.h"
#include "leveldb/write_batch.h"
#include "util/lock.h"
#include "util/thread.h"

static int hset_one(const SSDB *ssdb, const Bytes &name, const Bytes &key, const Bytes &val, char log_type);
static int hdel_one(const SSDB *ssdb, const Bytes &name, const Bytes &key, char log_type);
static int hincr_one(const SSDB *ssdb, const Bytes &name, const Bytes &key, int64_t by, std::string *new_val, char log_type);
static int incr_hsize(const SSDB *ssdb, const Bytes &name, int64_t incr);

/**
 * @return -1: error, 1: new item offered
 */
int SSDB::qoffer(const Bytes &name, const Bytes &val, char log_type){
	Transaction trans(binlogs);
	const Bytes tailKey("tail");
	std::string tailValue;
	const Bytes sizeKey("size");
	std::string sizeValue;
	int ret = hincr_one(this, name, tailKey, 1, &tailValue, log_type);
	if(ret >= 0){
		const Bytes key(tailValue);
		ret = hset_one(this, name, key, val, log_type);
		if(ret >= 0){
			ret = hincr_one(this, name, sizeKey, 1, &sizeValue, log_type);
			if(ret >= 0){
				leveldb::Status s = binlogs->commit();
				if(!s.ok()){
					return -1;
				}
				return 1;
			}
		}
	}
	return -1;
}

/**
 * @return -1: error, 0: no data, 1: one item polled
 */
int SSDB::qpoll(const Bytes &name, std::string *val, char log_type){
	Transaction trans(binlogs);
	int ret = -1;
	const Bytes headKey("head");
	std::string headValue;
	const Bytes sizeKey("size");
	std::string sizeValue;
	int64_t size = this->qsize(name);

	//no data
	if(size < 1){
		return size;
	}
	ret = hincr_one(this, name, headKey, 1, &headValue, log_type);
	//find one
	if(ret >= 0){
		const Bytes dataKey(headValue);
		//get data value
		ret = this->hget(name, dataKey, val);
		if(ret >= 0){
			ret = hincr_one(this, name, sizeKey, -1, &sizeValue, log_type);
			if(ret >= 0){
				ret = hdel_one(this, name, dataKey, log_type);
				if(ret >= 0){
					leveldb::Status s = binlogs->commit();
					if(!s.ok()){
						return -1;
					}
					return 1;
				}
			}
		}
	}
	return -1;
}

/**
 * @return -1: error, 0: no data, 1: one item peeked
 */
int SSDB::qpeek(const Bytes &name, std::string *val)const{
	Transaction trans(binlogs);
	int ret = -1;
	const Bytes headKey("head");
	std::string headValue;
	int64_t head = 0;
	int64_t size = this->qsize(name);

	//no data
	if(size < 1){
		return size;
	}
	ret = this->hget(name, headKey, &headValue);
	if(ret >= 0){
		head = str_to_int64(headValue.data(), headValue.size()) + 1;
		//get data value
		ret = this->hget(name, int64_to_str(head), val);
		return ret;
	}
	return -1;
}
/**
 * @return -1: error, queue size
 */
int64_t SSDB::qsize(const Bytes &name)const{
	const Bytes sizeKey("size");
	std::string sizeValue;
	int ret = this->hget(name, sizeKey, &sizeValue);
	if(1 == ret){//has data
		return str_to_int64(sizeValue.data(), sizeValue.size());
	}
	return ret;
}

static int hincr_one(const SSDB *ssdb, const Bytes &name, const Bytes &key, int64_t by, std::string *new_val, char log_type){
	int64_t val;
	std::string old;
	int ret = ssdb->hget(name, key, &old);
	if(ret == -1){
		return -1;
	}else if(ret == 0){
		val = by;
	}else{
		val = str_to_int64(old.data(), old.size()) + by;
	}

	*new_val = int64_to_str(val);
	ret = hset_one(ssdb, name, key, *new_val, log_type);
	if(ret >= 0){
		if(ret > 0){
			if(incr_hsize(ssdb, name, ret) == -1){
				return -1;
			}
		}
	}
	return ret;
}

// returns the number of newly added items
static int hset_one(const SSDB *ssdb, const Bytes &name, const Bytes &key, const Bytes &val, char log_type){
	if(name.empty() || key.empty()){
		log_error("empty name or key!");
		//return -1;
		return 0;
	}
	if(name.size() > SSDB_KEY_LEN_MAX ){
		log_error("name too long!");
		return -1;
	}
	if(key.size() > SSDB_KEY_LEN_MAX){
		log_error("key too long!");
		return -1;
	}
	int ret = 0;
	std::string dbval;
	if(ssdb->hget(name, key, &dbval) == 0){ // not found
		std::string hkey = encode_hash_key(name, key);
		ssdb->binlogs->Put(hkey, val.Slice());
		ssdb->binlogs->add(log_type, BinlogCommand::HSET, hkey);
		ret = 1;
	}else{
		if(dbval != val){
			std::string hkey = encode_hash_key(name, key);
			ssdb->binlogs->Put(hkey, val.Slice());
			ssdb->binlogs->add(log_type, BinlogCommand::HSET, hkey);
		}
		ret = 0;
	}
	return ret;
}

static int hdel_one(const SSDB *ssdb, const Bytes &name, const Bytes &key, char log_type){
	if(name.size() > SSDB_KEY_LEN_MAX ){
		log_error("name too long!");
		return -1;
	}
	if(key.size() > SSDB_KEY_LEN_MAX){
		log_error("key too long!");
		return -1;
	}
	std::string dbval;
	if(ssdb->hget(name, key, &dbval) == 0){
		return 0;
	}

	std::string hkey = encode_hash_key(name, key);
	ssdb->binlogs->Delete(hkey);
	ssdb->binlogs->add(log_type, BinlogCommand::HDEL, hkey);
	
	return 1;
}
static int incr_hsize(const SSDB *ssdb, const Bytes &name, int64_t incr){
	int64_t size = ssdb->hsize(name);
	size += incr;
	std::string size_key = encode_hsize_key(name);
	if(size == 0){
		ssdb->binlogs->Delete(size_key);
	}else{
		ssdb->binlogs->Put(size_key, leveldb::Slice((char *)&size, sizeof(int64_t)));
	}
	return 0;
}
