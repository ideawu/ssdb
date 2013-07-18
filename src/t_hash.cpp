#include "t_hash.h"
#include "ssdb.h"
#include "leveldb/write_batch.h"
#include "util/lock.h"
#include "util/thread.h"

static int hset_one(const SSDB *ssdb, const Bytes &name, const Bytes &key, const Bytes &val, char log_type);
static int hdel_one(const SSDB *ssdb, const Bytes &name, const Bytes &key, char log_type);
static int incr_hsize(SSDB *ssdb, const Bytes &name, int64_t incr);

int SSDB::multi_hset(const Bytes &name, const std::vector<Bytes> &kvs, int offset, char log_type){
	Transaction trans(binlogs);

	int ret = 0;
	std::vector<Bytes>::const_iterator it;
	it = kvs.begin() + offset;
	for(; it != kvs.end(); it += 2){
		const Bytes &key = *it;
		const Bytes &val = *(it + 1);
		int tmp = hset_one(this, name, key, val, log_type);
		if(tmp == -1){
			return -1;
		}
		ret += tmp;
	}
	if(ret >= 0){
		if(ret > 0){
			if(incr_hsize(this, name, ret) == -1){
				return -1;
			}
		}
		leveldb::Status s = binlogs->commit();
		if(!s.ok()){
			log_error("zdel error: %s", s.ToString().c_str());
			return -1;
		}
	}
	return ret;
}

int SSDB::multi_hdel(const Bytes &name, const std::vector<Bytes> &keys, int offset, char log_type){
	Transaction trans(binlogs);

	int ret = 0;
	std::vector<Bytes>::const_iterator it;
	it = keys.begin() + offset;
	for(; it != keys.end(); it++){
		const Bytes &key = *it;
		int tmp = hdel_one(this, name, key, log_type);
		if(tmp == -1){
			return -1;
		}
		ret += tmp;
	}
	if(ret >= 0){
		if(ret > 0){
			if(incr_hsize(this, name, -ret) == -1){
				return -1;
			}
		}
		leveldb::Status s = binlogs->commit();
		if(!s.ok()){
			log_error("zdel error: %s", s.ToString().c_str());
			return -1;
		}
	}
	return ret;
}

/**
 * @return -1: error, 0: item updated, 1: new item inserted
 */
int SSDB::hset(const Bytes &name, const Bytes &key, const Bytes &val, char log_type){
	Transaction trans(binlogs);

	int ret = hset_one(this, name, key, val, log_type);
	if(ret >= 0){
		if(ret > 0){
			if(incr_hsize(this, name, ret) == -1){
				return -1;
			}
		}
		leveldb::Status s = binlogs->commit();
		if(!s.ok()){
			return -1;
		}
	}
	return ret;
}

int SSDB::hdel(const Bytes &name, const Bytes &key, char log_type){
	Transaction trans(binlogs);

	int ret = hdel_one(this, name, key, log_type);
	if(ret >= 0){
		if(ret > 0){
			if(incr_hsize(this, name, -ret) == -1){
				return -1;
			}
		}
		leveldb::Status s = binlogs->commit();
		if(!s.ok()){
			return -1;
		}
	}
	return ret;
}

int SSDB::hincr(const Bytes &name, const Bytes &key, int64_t by, std::string *new_val, char log_type){
	Transaction trans(binlogs);

	int64_t val;
	std::string old;
	int ret = this->hget(name, key, &old);
	if(ret == -1){
		return -1;
	}else if(ret == 0){
		val = by;
	}else{
		val = str_to_int64(old.data(), old.size()) + by;
	}

	*new_val = int64_to_str(val);
	ret = hset_one(this, name, key, *new_val, log_type);
	if(ret >= 0){
		if(ret > 0){
			if(incr_hsize(this, name, ret) == -1){
				return -1;
			}
		}
		leveldb::Status s = binlogs->commit();
		if(!s.ok()){
			return -1;
		}
	}
	return ret;
}

int64_t SSDB::hsize(const Bytes &name) const{
	std::string size_key = encode_hsize_key(name);
	std::string val;
	leveldb::Status s;

	s = db->Get(leveldb::ReadOptions(), size_key, &val);
	if(s.IsNotFound()){
		return 0;
	}else if(!s.ok()){
		return -1;
	}else{
		if(val.size() != sizeof(uint64_t)){
			return 0;
		}
		int64_t ret = *(int64_t *)val.data();
		return ret < 0? 0 : ret;
	}
}

int SSDB::hget(const Bytes &name, const Bytes &key, std::string *val) const{
	std::string dbkey = encode_hash_key(name, key);
	leveldb::Status s = db->Get(leveldb::ReadOptions(), dbkey, val);
	if(s.IsNotFound()){
		return 0;
	}
	if(!s.ok()){
		return -1;
	}
	return 1;
}

HIterator* SSDB::hscan(const Bytes &name, const Bytes &start, const Bytes &end, int limit) const{
	std::string key_start, key_end;

	key_start = encode_hash_key(name, start);
	if(!end.empty()){
		key_end = encode_hash_key(name, end);
	}
	//dump(key_start.data(), key_start.size(), "scan.start");
	//dump(key_end.data(), key_end.size(), "scan.end");

	return new HIterator(this->iterator(key_start, key_end, limit), name);
}

HIterator* SSDB::hrscan(const Bytes &name, const Bytes &start, const Bytes &end, int limit) const{
	std::string key_start, key_end;

	key_start = encode_hash_key(name, start);
	if(start.empty()){
		key_start.append(1, 255);
	}
	if(!end.empty()){
		key_end = encode_hash_key(name, end);
	}
	//dump(key_start.data(), key_start.size(), "scan.start");
	//dump(key_end.data(), key_end.size(), "scan.end");

	return new HIterator(this->rev_iterator(key_start, key_end, limit), name);
}

int SSDB::hlist(const Bytes &name_s, const Bytes &name_e, int limit,
		std::vector<std::string> *list) const{
	std::string start;
	std::string end;
	start = encode_hsize_key(name_s);
	if(!name_e.empty()){
		end = encode_hsize_key(name_e);
	}
	Iterator *it = this->iterator(start, end, limit);
	while(it->next()){
		Bytes ks = it->key();
		if(ks.data()[0] != DataType::HSIZE){
			break;
		}
		std::string n;
		if(decode_hsize_key(ks, &n) == -1){
			continue;
		}
		list->push_back(n);
	}
	delete it;
	return 0;
}

// returns the number of newly added items
static int hset_one(const SSDB *ssdb, const Bytes &name, const Bytes &key, const Bytes &val, char log_type){
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

static int incr_hsize(SSDB *ssdb, const Bytes &name, int64_t incr){
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
