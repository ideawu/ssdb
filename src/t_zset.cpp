#include "t_zset.h"
#include "leveldb/write_batch.h"

static const char *SSDB_SCORE_MIN		= "-9223372036854775808";
static const char *SSDB_SCORE_MAX		= "+9223372036854775807";

int64_t SSDB::zsize(const Bytes &name) const{
	std::string size_key = encode_zsize_key(name);
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

int SSDB::zset(const Bytes &name, const Bytes &key, const Bytes &score) const{
	if(name.size() > SSDB_KEY_LEN_MAX || key.size() > SSDB_KEY_LEN_MAX){
		return -1;
	}

	std::string old_score;
	int found = this->zget(name, key, &old_score);
	if(found == 0 || old_score != score){
		leveldb::Status s;
		leveldb::WriteBatch batch;
		std::string k0, k1, k2, v;

		if(found == 0){
			// update zsize
			int64_t size = this->zsize(name);
			if(size == -1){
				return -1;
			}
			size ++;
			std::string size_key = encode_zsize_key(name);
			batch.Put(size_key, leveldb::Slice((char *)&size, sizeof(int64_t)));
		}

		// delete zset key
		k1 = encode_z_key(name, key, old_score);
		batch.Delete(k1);

		// update score
		k0 = encode_zs_key(name, key);
		v.assign(score.data(), score.size());
		batch.Put(k0, v);

		// add zset key
		k2 = encode_z_key(name, key, score);
		batch.Put(k2, "");

		s = db->Write(write_options, &batch);
		if(!s.ok()){
			log_error("zset error: %s", s.ToString().c_str());
			return -1;
		}
		return 1;
	}
	return 0;
}

int SSDB::zget(const Bytes &name, const Bytes &key, std::string *score) const{
	std::string buf = encode_zs_key(name, key);
	leveldb::Status s = db->Get(read_options, buf, score);
	if(s.IsNotFound()){
		return 0;
	}
	if(!s.ok()){
		log_error("zget error: %s", s.ToString().c_str());
		return -1;
	}
	return 1;
}

int SSDB::zdel(const Bytes &name, const Bytes &key) const{
	std::string old_score;
	int found = this->zget(name, key, &old_score);
	if(found != 1){
		return 0;
	}

	leveldb::Status s;
	leveldb::WriteBatch batch;
	std::string k0, k1;

	// update zsize
	int64_t size = this->zsize(name);
	if(size == -1){
		return -1;
	}
	size --;
	std::string size_key = encode_zsize_key(name);
	batch.Put(size_key, leveldb::Slice((char *)&size, sizeof(int64_t)));

	// delete score
	k0 = encode_zs_key(name, key);
	batch.Delete(k0);

	// delete zset key
	k1 = encode_z_key(name, key, old_score);
	batch.Delete(k1);

	s = db->Write(write_options, &batch);
	if(!s.ok()){
		log_error("zdel error: %s", s.ToString().c_str());
		return -1;
	}
	return 1;
}

ZIterator* SSDB::zscan(const Bytes &name, const Bytes &key,
		const Bytes &score_start, const Bytes &score_end, int limit) const{
	std::string key_start, key_end;

	if(score_start.empty()){
		key_start = encode_z_key(name, key, SSDB_SCORE_MIN);
	}else{
		key_start = encode_z_key(name, key, score_start);
	}
	if(score_end.empty()){
		key_end = encode_z_key(name, "", SSDB_SCORE_MAX);
	}else{
		key_end = encode_z_key(name, "", score_end);
	}

	//dump(key_start.data(), key_start.size(), "zscan.start");
	//dump(key_end.data(), key_end.size(), "zscan.end");

	return new ZIterator(this->iterator(key_start, key_end, limit), name);
}

int SSDB::zincr(const Bytes &name, const Bytes &key, int64_t by, std::string *new_val) const{
	int64_t val;
	std::string old;
	int ret = this->zget(name, key, &old);
	if(ret == -1){
		return -1;
	}else if(ret == 0){
		val = by;
	}else{
		val = str_to_int64(old.data(), old.size()) + by;
	}

	*new_val = int64_to_str(val);
	return this->zset(name, key, *new_val);
}
