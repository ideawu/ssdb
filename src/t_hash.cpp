#include "t_hash.h"
#include "ssdb.h"

int64_t SSDB::hsize(const Bytes &name) const{
	std::string size_key = encode_hsize_key(name);
	std::string val;
	leveldb::Status s;

	s = db->Get(read_options, size_key, &val);
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

int SSDB::hset(const Bytes &name, const Bytes &key, const Bytes &val) const{
	std::string hkey = encode_hash_key(name, key);
	std::string dbval;
	leveldb::Status s;
	leveldb::WriteBatch batch;

	if(this->hget(name, key, &dbval) == 0){
		int64_t size = this->hsize(name);
		if(size == -1){
			return -1;
		}
		size ++;
		std::string size_key = encode_hsize_key(name);
		batch.Put(size_key, leveldb::Slice((char *)&size, sizeof(int64_t)));
	}
	if(dbval != val){
		batch.Put(hkey, val.Slice());
	}

	s = db->Write(write_options, &batch);
	if(!s.ok()){
		return -1;
	}
	return 1;
}

int SSDB::hget(const Bytes &name, const Bytes &key, std::string *val) const{
	std::string dbkey = encode_hash_key(name, key);

	leveldb::Status s = db->Get(read_options, dbkey, val);
	if(s.IsNotFound()){
		return 0;
	}
	if(!s.ok()){
		return -1;
	}
	return 1;
}

int SSDB::hdel(const Bytes &name, const Bytes &key) const{
	std::string hkey = encode_hash_key(name, key);
	std::string dbval;
	leveldb::Status s;
	leveldb::WriteBatch batch;

	if(this->hget(name, key, &dbval) == 0){
		return 0;
	}

	int64_t size = this->hsize(name);
	if(size == -1){
		return -1;
	}
	size --;
	std::string size_key = encode_hsize_key(name);
	if(size == 0){
		batch.Delete(size_key);
	}else{
		batch.Put(size_key, leveldb::Slice((char *)&size, sizeof(int64_t)));
	}
	batch.Delete(hkey);

	s = db->Write(write_options, &batch);
	if(!s.ok()){
		return -1;
	}
	return 1;
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
	if(!end.empty()){
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
