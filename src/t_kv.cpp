#include "t_kv.h"
#include "leveldb/write_batch.h"

int SSDB::multi_set(const std::vector<Bytes> &kvs, int offset) const{
	binlogs->begin();
	std::vector<Bytes>::const_iterator it;
	it = kvs.begin() + offset;
	for(; it != kvs.end(); it += 2){
		const Bytes &key = *it;
		const Bytes &val = *(it + 1);
		std::string buf = encode_kv_key(key);
		binlogs->Put(buf, val.Slice());
		binlogs->log(BinlogType::SYNC, BinlogCommand::KSET, buf);
	}
	leveldb::Status s = binlogs->commit();
	if(!s.ok()){
		log_error("multi_set error: %s", s.ToString().c_str());
		return -1;
	}
	return kvs.size() - offset;
}

int SSDB::multi_del(const std::vector<Bytes> &keys, int offset) const{
	binlogs->begin();
	for(; offset < (int)keys.size(); offset += 1){
		const Bytes &key = keys[offset];
		std::string buf = encode_kv_key(key);
		binlogs->Delete(buf);
		binlogs->log(BinlogType::SYNC, BinlogCommand::KDEL, buf);
	}
	leveldb::Status s = binlogs->commit();
	if(!s.ok()){
		log_error("multi_del error: %s", s.ToString().c_str());
		return -1;
	}
	return keys.size() - offset;
}

int SSDB::set(const Bytes &key, const Bytes &val) const{
	std::string buf = encode_kv_key(key);
	binlogs->begin();
	binlogs->Put(buf, val.Slice());
	binlogs->log(BinlogType::SYNC, BinlogCommand::KSET, buf);
	leveldb::Status s = binlogs->commit();
	if(!s.ok()){
		log_error("set error: %s", s.ToString().c_str());
		return -1;
	}
	return 1;
}

int SSDB::get(const Bytes &key, std::string *val) const{
	std::string buf = encode_kv_key(key);
	leveldb::ReadOptions read_opts;

	leveldb::Status s = db->Get(read_opts, buf, val);
	if(s.IsNotFound()){
		return 0;
	}
	if(!s.ok()){
		log_error("get error: %s", s.ToString().c_str());
		return -1;
	}
	return 1;
}

int SSDB::del(const Bytes &key) const{
	std::string buf = encode_kv_key(key);
	binlogs->begin();
	binlogs->Delete(buf);
	binlogs->log(BinlogType::SYNC, BinlogCommand::KDEL, buf);
	leveldb::Status s = binlogs->commit();
	if(!s.ok()){
		log_error("del error: %s", s.ToString().c_str());
		return -1;
	}
	return 1;
}

int SSDB::incr(const Bytes &key, int64_t by, std::string *new_val) const{
	int64_t val;
	std::string old;
	int ret = this->get(key, &old);
	if(ret == -1){
		return -1;
	}else if(ret == 0){
		val = by;
	}else{
		val = str_to_int64(old.data(), old.size()) + by;
	}

	*new_val = int64_to_str(val);
	return this->set(key, *new_val);
}

KIterator* SSDB::scan(const Bytes &start, const Bytes &end, int limit) const{
	std::string key_start, key_end;

	key_start = encode_kv_key(start);
	if(end.empty()){
		key_end = "";
	}else{
		key_end = encode_kv_key(end);
	}
	//dump(key_start.data(), key_start.size(), "scan.start");
	//dump(key_end.data(), key_end.size(), "scan.end");

	return new KIterator(this->iterator(key_start, key_end, limit));
}

KIterator* SSDB::rscan(const Bytes &start, const Bytes &end, int limit) const{
	std::string key_start, key_end;

	key_start = encode_kv_key(start);
	if(start.empty()){
		key_start.append(1, 255);
	}
	if(end.empty()){
		key_end = "";
	}else{
		key_end = encode_kv_key(end);
	}
	//dump(key_start.data(), key_start.size(), "scan.start");
	//dump(key_end.data(), key_end.size(), "scan.end");

	return new KIterator(this->rev_iterator(key_start, key_end, limit));
}
