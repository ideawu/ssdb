/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "t_hash.h"

static int hset_one(SSDBImpl *ssdb, const Bytes &name, const Bytes &key, const Bytes &val, char log_type);
static int hdel_one(SSDBImpl *ssdb, const Bytes &name, const Bytes &key, char log_type);
static int incr_hsize(SSDBImpl *ssdb, const Bytes &name, int64_t incr);

/**
 * @return -1: error, 0: item updated, 1: new item inserted
 */
int SSDBImpl::hset(const Bytes &name, const Bytes &key, const Bytes &val, char log_type){
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

int SSDBImpl::hdel(const Bytes &name, const Bytes &key, char log_type){
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

int SSDBImpl::hincr(const Bytes &name, const Bytes &key, int64_t by, int64_t *new_val, char log_type){
	Transaction trans(binlogs);

	std::string old;
	int ret = this->hget(name, key, &old);
	if(ret == -1){
		return -1;
	}else if(ret == 0){
		*new_val = by;
	}else{
		*new_val = str_to_int64(old) + by;
		if(errno != 0){
			return 0;
		}
	}

	ret = hset_one(this, name, key, str(*new_val), log_type);
	if(ret == -1){
		return -1;
	}
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
	return 1;
}

int64_t SSDBImpl::hsize(const Bytes &name){
	std::string size_key = encode_hsize_key(name);
	std::string val;
	leveldb::Status s;

	s = ldb->Get(leveldb::ReadOptions(), size_key, &val);
	if(s.IsNotFound()){
		return 0;
	}else if(!s.ok()){
		return -1;
	}else{
		if(val.size() != sizeof(uint64_t)){
			return 0;
		}
		int64_t ret; 
		memcpy(&ret, val.data(), sizeof(int64_t));
		return ret < 0? 0 : ret;
	}
}

int64_t SSDBImpl::hclear(const Bytes &name){
	int64_t count = 0;
	std::string start;
	while(1){
		HIterator *it = this->hscan(name, start, "", 10000);
		int num = 0;
		while(it->next()){
			start = it->key;
			int ret = this->hdel(name, it->key);
			if(ret == -1){
				delete it;
				return 0;
			}
			num ++;
		};
		delete it;

		if(num == 0){
			break;
		}
		count += num;
	}
	return count;
}

int SSDBImpl::hget(const Bytes &name, const Bytes &key, std::string *val){
	std::string dbkey = encode_hash_key(name, key);
	leveldb::Status s = ldb->Get(leveldb::ReadOptions(), dbkey, val);
	if(s.IsNotFound()){
		return 0;
	}
	if(!s.ok()){
		log_error("%s", s.ToString().c_str());
		return -1;
	}
	return 1;
}

HIterator* SSDBImpl::hscan(const Bytes &name, const Bytes &start, const Bytes &end, uint64_t limit){
	std::string key_start, key_end;

	key_start = encode_hash_key(name, start);
	if(!end.empty()){
		key_end = encode_hash_key(name, end);
	}
	//dump(key_start.data(), key_start.size(), "scan.start");
	//dump(key_end.data(), key_end.size(), "scan.end");

	return new HIterator(this->iterator(key_start, key_end, limit), name);
}

HIterator* SSDBImpl::hrscan(const Bytes &name, const Bytes &start, const Bytes &end, uint64_t limit){
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

static void get_hnames(Iterator *it, std::vector<std::string> *list){
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
}

int SSDBImpl::hlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
		std::vector<std::string> *list){
	std::string start;
	std::string end;
	
	start = encode_hsize_key(name_s);
	if(!name_e.empty()){
		end = encode_hsize_key(name_e);
	}
	
	Iterator *it = this->iterator(start, end, limit);
	get_hnames(it, list);
	delete it;
	return 0;
}

int SSDBImpl::hrlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
		std::vector<std::string> *list){
	std::string start;
	std::string end;
	
	start = encode_hsize_key(name_s);
	if(name_s.empty()){
		start.append(1, 255);
	}
	if(!name_e.empty()){
		end = encode_hsize_key(name_e);
	}
	
	Iterator *it = this->rev_iterator(start, end, limit);
	get_hnames(it, list);
	delete it;
	return 0;
}

// returns the number of newly added items
static int hset_one(SSDBImpl *ssdb, const Bytes &name, const Bytes &key, const Bytes &val, char log_type){
	if(name.empty() || key.empty()){
		log_error("empty name or key!");
		return -1;
	}
	if(name.size() > SSDB_KEY_LEN_MAX ){
		log_error("name too long! %s", hexmem(name.data(), name.size()).c_str());
		return -1;
	}
	if(key.size() > SSDB_KEY_LEN_MAX){
		log_error("key too long! %s", hexmem(key.data(), key.size()).c_str());
		return -1;
	}
	int ret = 0;
	std::string dbval;
	if(ssdb->hget(name, key, &dbval) == 0){ // not found
		std::string hkey = encode_hash_key(name, key);
		ssdb->binlogs->Put(hkey, slice(val));
		ssdb->binlogs->add_log(log_type, BinlogCommand::HSET, hkey);
		ret = 1;
	}else{
		if(dbval != val){
			std::string hkey = encode_hash_key(name, key);
			ssdb->binlogs->Put(hkey, slice(val));
			ssdb->binlogs->add_log(log_type, BinlogCommand::HSET, hkey);
		}
		ret = 0;
	}
	return ret;
}

static int hdel_one(SSDBImpl *ssdb, const Bytes &name, const Bytes &key, char log_type){
	if(name.size() > SSDB_KEY_LEN_MAX ){
		log_error("name too long! %s", hexmem(name.data(), name.size()).c_str());
		return -1;
	}
	if(key.size() > SSDB_KEY_LEN_MAX){
		log_error("key too long! %s", hexmem(key.data(), key.size()).c_str());
		return -1;
	}
	std::string dbval;
	if(ssdb->hget(name, key, &dbval) == 0){
		return 0;
	}

	std::string hkey = encode_hash_key(name, key);
	ssdb->binlogs->Delete(hkey);
	ssdb->binlogs->add_log(log_type, BinlogCommand::HDEL, hkey);
	
	return 1;
}

static int incr_hsize(SSDBImpl *ssdb, const Bytes &name, int64_t incr){
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

int64_t SSDBImpl::hfix(const Bytes &name){
	Transaction trans(binlogs);

	uint64_t size = 0;
	HIterator *it = this->hscan(name, "", "", UINT64_MAX);
	while(it->next()){
		size ++;
	}
	delete it;

	std::string size_key = encode_hsize_key(name);
	if(size == 0){
		ldb->Delete(leveldb::WriteOptions(), size_key);
	}else{
		ldb->Put(leveldb::WriteOptions(), size_key, leveldb::Slice((char *)&size, sizeof(int64_t)));
	}
	
	return size;
}
