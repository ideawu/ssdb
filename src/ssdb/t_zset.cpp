/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include <limits.h>
#include "t_zset.h"

static const char *SSDB_SCORE_MIN		= "-9223372036854775808";
static const char *SSDB_SCORE_MAX		= "+9223372036854775807";

static int zset_one(SSDBImpl *ssdb, const Bytes &name, const Bytes &key, const Bytes &score, char log_type);
static int zdel_one(SSDBImpl *ssdb, const Bytes &name, const Bytes &key, char log_type);
static int incr_zsize(SSDBImpl *ssdb, const Bytes &name, int64_t incr);

/**
 * @return -1: error, 0: item updated, 1: new item inserted
 */
int SSDBImpl::zset(const Bytes &name, const Bytes &key, const Bytes &score, char log_type){
	Transaction trans(binlogs);

	int ret = zset_one(this, name, key, score, log_type);
	if(ret >= 0){
		if(ret > 0){
			if(incr_zsize(this, name, ret) == -1){
				return -1;
			}
		}
		leveldb::Status s = binlogs->commit();
		if(!s.ok()){
			log_error("zset error: %s", s.ToString().c_str());
			return -1;
		}
	}
	return ret;
}

int SSDBImpl::zdel(const Bytes &name, const Bytes &key, char log_type){
	Transaction trans(binlogs);

	int ret = zdel_one(this, name, key, log_type);
	if(ret >= 0){
		if(ret > 0){
			if(incr_zsize(this, name, -ret) == -1){
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

int SSDBImpl::zincr(const Bytes &name, const Bytes &key, int64_t by, int64_t *new_val, char log_type){
	Transaction trans(binlogs);

	std::string old;
	int ret = this->zget(name, key, &old);
	if(ret == -1){
		return -1;
	}else if(ret == 0){
		*new_val = by;
	}else{
		*new_val = str_to_int64(old) + by;
	}

	ret = zset_one(this, name, key, str(*new_val), log_type);
	if(ret == -1){
		return -1;
	}
	if(ret >= 0){
		if(ret > 0){
			if(incr_zsize(this, name, ret) == -1){
				return -1;
			}
		}
		leveldb::Status s = binlogs->commit();
		if(!s.ok()){
			log_error("zset error: %s", s.ToString().c_str());
			return -1;
		}
	}
	return 1;
}

int64_t SSDBImpl::zsize(const Bytes &name){
	std::string size_key = encode_zsize_key(name);
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

int SSDBImpl::zget(const Bytes &name, const Bytes &key, std::string *score){
	std::string buf = encode_zset_key(name, key);
	leveldb::Status s = ldb->Get(leveldb::ReadOptions(), buf, score);
	if(s.IsNotFound()){
		return 0;
	}
	if(!s.ok()){
		log_error("zget error: %s", s.ToString().c_str());
		return -1;
	}
	return 1;
}

static ZIterator* ziterator(
	SSDBImpl *ssdb,
	const Bytes &name, const Bytes &key_start,
	const Bytes &score_start, const Bytes &score_end,
	uint64_t limit, Iterator::Direction direction)
{
	if(direction == Iterator::FORWARD){
		std::string start, end;
		if(score_start.empty()){
			start = encode_zscore_key(name, key_start, SSDB_SCORE_MIN);
		}else{
			start = encode_zscore_key(name, key_start, score_start);
		}
		if(score_end.empty()){
			end = encode_zscore_key(name, "\xff", SSDB_SCORE_MAX);
		}else{
			end = encode_zscore_key(name, "\xff", score_end);
		}
		return new ZIterator(ssdb->iterator(start, end, limit), name);
	}else{
		std::string start, end;
		if(score_start.empty()){
			start = encode_zscore_key(name, key_start, SSDB_SCORE_MAX);
		}else{
			if(key_start.empty()){
				start = encode_zscore_key(name, "\xff", score_start);
			}else{
				start = encode_zscore_key(name, key_start, score_start);
			}
		}
		if(score_end.empty()){
			end = encode_zscore_key(name, "", SSDB_SCORE_MIN);
		}else{
			end = encode_zscore_key(name, "", score_end);
		}
		return new ZIterator(ssdb->rev_iterator(start, end, limit), name);
	}
}

int64_t SSDBImpl::zrank(const Bytes &name, const Bytes &key){
	ZIterator *it = ziterator(this, name, "", "", "", INT_MAX, Iterator::FORWARD);
	uint64_t ret = 0;
	while(true){
		if(it->next() == false){
			ret = -1;
			break;
		}
		if(key == it->key){
			break;
		}
		ret ++;
	}
	delete it;
	return ret;
}

int64_t SSDBImpl::zrrank(const Bytes &name, const Bytes &key){
	ZIterator *it = ziterator(this, name, "", "", "", INT_MAX, Iterator::BACKWARD);
	uint64_t ret = 0;
	while(true){
		if(it->next() == false){
			ret = -1;
			break;
		}
		if(key == it->key){
			break;
		}
		ret ++;
	}
	delete it;
	return ret;
}

ZIterator* SSDBImpl::zrange(const Bytes &name, uint64_t offset, uint64_t limit){
	if(offset + limit > limit){
		limit = offset + limit;
	}
	ZIterator *it = ziterator(this, name, "", "", "", limit, Iterator::FORWARD);
	it->skip(offset);
	return it;
}

ZIterator* SSDBImpl::zrrange(const Bytes &name, uint64_t offset, uint64_t limit){
	if(offset + limit > limit){
		limit = offset + limit;
	}
	ZIterator *it = ziterator(this, name, "", "", "", limit, Iterator::BACKWARD);
	it->skip(offset);
	return it;
}

ZIterator* SSDBImpl::zscan(const Bytes &name, const Bytes &key,
		const Bytes &score_start, const Bytes &score_end, uint64_t limit)
{
	std::string score;
	// if only key is specified, load its value
	if(!key.empty() && score_start.empty()){
		this->zget(name, key, &score);
	}else{
		score = score_start.String();
	}
	return ziterator(this, name, key, score, score_end, limit, Iterator::FORWARD);
}

ZIterator* SSDBImpl::zrscan(const Bytes &name, const Bytes &key,
		const Bytes &score_start, const Bytes &score_end, uint64_t limit)
{
	std::string score;
	// if only key is specified, load its value
	if(!key.empty() && score_start.empty()){
		this->zget(name, key, &score);
	}else{
		score = score_start.String();
	}
	return ziterator(this, name, key, score, score_end, limit, Iterator::BACKWARD);
}

static void get_znames(Iterator *it, std::vector<std::string> *list){
	while(it->next()){
		Bytes ks = it->key();
		//dump(ks.data(), ks.size());
		if(ks.data()[0] != DataType::ZSIZE){
			break;
		}
		std::string n;
		if(decode_zsize_key(ks, &n) == -1){
			continue;
		}
		list->push_back(n);
	}
}

int SSDBImpl::zlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
		std::vector<std::string> *list){
	std::string start;
	std::string end;
	
	start = encode_zsize_key(name_s);
	if(!name_e.empty()){
		end = encode_zsize_key(name_e);
	}
	
	Iterator *it = this->iterator(start, end, limit);
	get_znames(it, list);
	delete it;
	return 0;
}

int SSDBImpl::zrlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
		std::vector<std::string> *list){
	std::string start;
	std::string end;

	start = encode_zsize_key(name_s);
	if(name_s.empty()){
		start.append(1, 255);
	}
	if(!name_e.empty()){
		end = encode_zsize_key(name_e);
	}

	Iterator *it = this->rev_iterator(start, end, limit);
	get_znames(it, list);
	delete it;
	return 0;
}

static std::string filter_score(const Bytes &score){
	int64_t s = score.Int64();
	return str(s);
}

// returns the number of newly added items
static int zset_one(SSDBImpl *ssdb, const Bytes &name, const Bytes &key, const Bytes &score, char log_type){
	if(name.empty() || key.empty()){
		log_error("empty name or key!");
		return 0;
		//return -1;
	}
	if(name.size() > SSDB_KEY_LEN_MAX ){
		log_error("name too long!");
		return -1;
	}
	if(key.size() > SSDB_KEY_LEN_MAX){
		log_error("key too long!");
		return -1;
	}
	std::string new_score = filter_score(score);
	std::string old_score;
	int found = ssdb->zget(name, key, &old_score);
	if(found == 0 || old_score != new_score){
		std::string k0, k1, k2;

		if(found){
			// delete zscore key
			k1 = encode_zscore_key(name, key, old_score);
			ssdb->binlogs->Delete(k1);
		}

		// add zscore key
		k2 = encode_zscore_key(name, key, new_score);
		ssdb->binlogs->Put(k2, "");

		// update zset
		k0 = encode_zset_key(name, key);
		ssdb->binlogs->Put(k0, new_score);
		ssdb->binlogs->add_log(log_type, BinlogCommand::ZSET, k0);

		return found? 0 : 1;
	}
	return 0;
}

static int zdel_one(SSDBImpl *ssdb, const Bytes &name, const Bytes &key, char log_type){
	if(name.size() > SSDB_KEY_LEN_MAX ){
		log_error("name too long!");
		return -1;
	}
	if(key.size() > SSDB_KEY_LEN_MAX){
		log_error("key too long!");
		return -1;
	}
	std::string old_score;
	int found = ssdb->zget(name, key, &old_score);
	if(found != 1){
		return 0;
	}

	std::string k0, k1;
	// delete zscore key
	k1 = encode_zscore_key(name, key, old_score);
	ssdb->binlogs->Delete(k1);

	// delete zset
	k0 = encode_zset_key(name, key);
	ssdb->binlogs->Delete(k0);
	ssdb->binlogs->add_log(log_type, BinlogCommand::ZDEL, k0);

	return 1;
}

static int incr_zsize(SSDBImpl *ssdb, const Bytes &name, int64_t incr){
	int64_t size = ssdb->zsize(name);
	size += incr;
	std::string size_key = encode_zsize_key(name);
	if(size == 0){
		ssdb->binlogs->Delete(size_key);
	}else{
		ssdb->binlogs->Put(size_key, leveldb::Slice((char *)&size, sizeof(int64_t)));
	}
	return 0;
}

int64_t SSDBImpl::zfix(const Bytes &name){
	Transaction trans(binlogs);
	std::string it_start, it_end;
	Iterator *it;
	leveldb::Status s;
	int64_t size = 0;
	int64_t old_size;

	it_start = encode_zscore_key(name, "", SSDB_SCORE_MIN);
	it_end = encode_zscore_key(name, "\xff", SSDB_SCORE_MAX);
	it = this->iterator(it_start, it_end, UINT64_MAX);
	size = 0;
	while(it->next()){
		Bytes ks = it->key();
		//Bytes vs = it->val();
		//dump(ks.data(), ks.size(), "z.next");
		//dump(vs.data(), vs.size(), "z.next");
		if(ks.data()[0] != DataType::ZSCORE){
			break;
		}
		std::string name2, key, score;
		if(decode_zscore_key(ks, &name2, &key, &score) == -1){
			size = -1;
			break;
		}
		if(name != name2){
			break;
		}
		size ++;
		
		std::string buf = encode_zset_key(name, key);
		std::string score2;
		s = ldb->Get(leveldb::ReadOptions(), buf, &score2);
		if(!s.ok() && !s.IsNotFound()){
			log_error("zget error: %s", s.ToString().c_str());
			size = -1;
			break;
		}
		if(s.IsNotFound() || score != score2){
			log_info("fix incorrect zset item, name: %s, key: %s, score: %s",
				hexmem(name.data(), name.size()).c_str(),
				hexmem(key.data(), key.size()).c_str(),
				hexmem(score.data(), score.size()).c_str()
				);
			s = ldb->Put(leveldb::WriteOptions(), buf, score);
			if(!s.ok()){
				log_error("db error! %s", s.ToString().c_str());
				size = -1;
				break;
			}
		}
	}
	delete it;
	if(size == -1){
		return -1;
	}

	old_size = this->zsize(name);
	if(old_size == -1){
		return -1;
	}
	if(old_size != size){
		log_info("fix zsize, name: %s, size: %" PRId64 " => %" PRId64,
			hexmem(name.data(), name.size()).c_str(), old_size, size);
		std::string size_key = encode_zsize_key(name);
		if(size == 0){
			s = ldb->Delete(leveldb::WriteOptions(), size_key);
		}else{
			s = ldb->Put(leveldb::WriteOptions(), size_key, leveldb::Slice((char *)&size, sizeof(int64_t)));
		}
	}
	
	//////////////////////////////////////////

	it_start = encode_zset_key(name, "");
	it_end = encode_zset_key(name.String() + "\xff", "");
	it = this->iterator(it_start, it_end, UINT64_MAX);
	size = 0;
	while(it->next()){
		Bytes ks = it->key();
		//Bytes vs = it->val();
		//dump(ks.data(), ks.size(), "z.next");
		//dump(vs.data(), vs.size(), "z.next");
		if(ks.data()[0] != DataType::ZSET){
			break;
		}
		std::string name2, key;
		if(decode_zset_key(ks, &name2, &key) == -1){
			size = -1;
			break;
		}
		if(name != name2){
			break;
		}
		size ++;
		Bytes score = it->val();
		
		std::string buf = encode_zscore_key(name, key, score);
		std::string score2;
		s = ldb->Get(leveldb::ReadOptions(), buf, &score2);
		if(!s.ok() && !s.IsNotFound()){
			log_error("zget error: %s", s.ToString().c_str());
			size = -1;
			break;
		}
		if(s.IsNotFound()){
			log_info("fix incorrect zset score, name: %s, key: %s, score: %s",
				hexmem(name.data(), name.size()).c_str(),
				hexmem(key.data(), key.size()).c_str(),
				hexmem(score.data(), score.size()).c_str()
				);
			s = ldb->Put(leveldb::WriteOptions(), buf, "");
			if(!s.ok()){
				log_error("db error! %s", s.ToString().c_str());
				size = -1;
				break;
			}
		}
	}
	delete it;
	if(size == -1){
		return -1;
	}

	old_size = this->zsize(name);
	if(old_size == -1){
		return -1;
	}
	if(old_size != size){
		log_info("fix zsize, name: %s, size: %" PRId64 " => %" PRId64,
			hexmem(name.data(), name.size()).c_str(), old_size, size);
		std::string size_key = encode_zsize_key(name);
		if(size == 0){
			s = ldb->Delete(leveldb::WriteOptions(), size_key);
		}else{
			s = ldb->Put(leveldb::WriteOptions(), size_key, leveldb::Slice((char *)&size, sizeof(int64_t)));
		}
	}
	
	//////////////////////////////////////////
	
	return size;
}
