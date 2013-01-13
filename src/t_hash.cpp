#include "t_hash.h"
#include "ssdb.h"

int SSDB::hset(const Bytes &name, const Bytes &key, const Bytes &val) const{
	std::string buf = encode_hash_key(name, key);

	leveldb::Status s = db->Put(write_options, buf, val.Slice());
	if(!s.ok()){
		return -1;
	}
	return 1;
}

int SSDB::hget(const Bytes &name, const Bytes &key, std::string *val) const{
	std::string buf = encode_hash_key(name, key);

	leveldb::Status s = db->Get(read_options, buf, val);
	if(s.IsNotFound()){
		return 0;
	}
	if(!s.ok()){
		return -1;
	}
	return 1;
}

int SSDB::hdel(const Bytes &name, const Bytes &key) const{
	std::string buf = encode_hash_key(name, key);

	leveldb::Status s = db->Delete(write_options, buf);
	if(!s.ok()){
		return -1;
	}
	return 1;
}

HIterator* SSDB::hscan(const Bytes &name, const Bytes &start, const Bytes &end, int limit) const{
	std::string key_start, key_end;

	key_start = encode_hash_key(name, start);
	if(end.empty()){
		key_end = "";
	}else{
		key_end = encode_hash_key(name, end);
	}
	//dump(key_start.data(), key_start.size(), "scan.start");
	//dump(key_end.data(), key_end.size(), "scan.end");

	return new HIterator(this->iterator(key_start, key_end, limit), name);
}

int SSDB::hincr(const Bytes &name, const Bytes &key, int64_t by, std::string *new_val) const{
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
	return this->hset(name, key, *new_val);
}
