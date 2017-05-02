/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include <limits.h>
#include "t_nset.h"

static const char *SSDB_SCORE_MIN		= "-9223372036854775808";
static const char *SSDB_SCORE_MAX		= "+9223372036854775807";

static int nset_one(SSDBImpl *ssdb, const Bytes &name, const Bytes &val, const Bytes &score, char log_type);
static int incr_nsize(SSDBImpl *ssdb, const Bytes &name, int64_t incr);

/**
 * @return -1: error, 0: item updated, 1: new item inserted
 */
int SSDBImpl::nset(const Bytes &name, const Bytes &val, const Bytes &score, char log_type){
	Transaction trans(binlogs);

	int ret = nset_one(this, name, val, score, log_type);
	if(ret >= 0){
		if(ret > 0){
			if(incr_nsize(this, name, ret) == -1){
				return -1;
			}
		}
		leveldb::Status s = binlogs->commit();
		if(!s.ok()){
			log_error("nset error: %s", s.ToString().c_str());
			return -1;
		}
	}
	return ret;
}

int64_t SSDBImpl::nsize(const Bytes &name){
	std::string size_key = encode_nsize_key(name);
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
		int64_t ret = *(int64_t *)val.data();
		return ret < 0? 0 : ret;
	}
}

static int ndel_one(SSDBImpl *ssdb, const Bytes &name, const Bytes &score, char log_type){
	if(name.size() > SSDB_KEY_LEN_MAX ){
		log_error("name too long!");
		return -1;
	}

	std::string nkey;
	// delete zscore key
	nkey = encode_nscore_key(name, score);
	std::string val;

	int found = ssdb->nget(name, score, &val);
	if(found != 1){
		return 0;
	}

	ssdb->binlogs->Delete(nkey);
	ssdb->binlogs->add_log(log_type, BinlogCommand::NDEL, nkey);

	return 1;
}

int SSDBImpl::nexpire_del(const Bytes &nkey, char log_type){
	Transaction trans(binlogs);

	std::string name, score;

	int ret = decode_nscore_key(nkey, &name, &score);
	if(ret != 0){
		return -1;
	}

	ret = ndel_one(this, Bytes(name), Bytes(score), log_type);
	if(ret >= 0){
		if(ret > 0){
			if(incr_nsize(this, Bytes(name), -ret) == -1){
				return -1;
			}
		}
		leveldb::Status s = binlogs->commit();
		if(!s.ok()){
			log_error("ndel error: %s", s.ToString().c_str());
			return -1;
		}
	}
	return ret;
}

int SSDBImpl::ndel(const Bytes &name, const Bytes &score, char log_type){
	Transaction trans(binlogs);

	int ret = ndel_one(this, name, score, log_type);
	if(ret >= 0){
		if(ret > 0){
			if(incr_nsize(this, name, -ret) == -1){
				return -1;
			}
		}
		leveldb::Status s = binlogs->commit();
		if(!s.ok()){
			log_error("ndel error: %s", s.ToString().c_str());
			return -1;
		}
	}
	return ret;
}

static NIterator* niterator(
	SSDBImpl *ssdb,
	const Bytes &name,
	const Bytes &score_start, const Bytes &score_end,
	uint64_t limit, Iterator::Direction direction)
{
	if(direction == Iterator::FORWARD){
		std::string start, end;
		if(score_start.empty()){
			start = encode_nscore_key(name, SSDB_SCORE_MIN);
		}else{
			start = encode_nscore_key(name, score_start);
		}
		if(score_end.empty()){
			end = encode_nscore_key(name, SSDB_SCORE_MAX);
		}else{
			end = encode_nscore_key(name, score_end);
		}
		return new NIterator(ssdb->iterator_ge(start, end, limit), name);
	}else{
		std::string start, end;
		if(score_start.empty()){
			start = encode_nscore_key(name, SSDB_SCORE_MAX);
		}else{
			start = encode_nscore_key(name, score_start);
		}
		if(score_end.empty()){
			end = encode_nscore_key(name, SSDB_SCORE_MIN);
		}else{
			end = encode_nscore_key(name, score_end);
		}
		return new NIterator(ssdb->rev_iterator(start, end, limit), name);
	}
}

NIterator* SSDBImpl::nrange(const Bytes &name, uint64_t offset, uint64_t limit){
	if(offset + limit > limit){
		limit = offset + limit;
	}
	NIterator *it = niterator(this, name, "", "", limit, Iterator::FORWARD);
	it->skip(offset);
	return it;
}

NIterator* SSDBImpl::nrrange(const Bytes &name, uint64_t offset, uint64_t limit){
	if(offset + limit > limit){
		limit = offset + limit;
	}
	NIterator *it = niterator(this, name, "", "", limit, Iterator::BACKWARD);
	it->skip(offset);
	return it;
}

NIterator* SSDBImpl::nscan(const Bytes &name,
		const Bytes &score_start, const Bytes &score_end, uint64_t limit)
{

	return niterator(this, name, score_start, score_end, limit, Iterator::FORWARD);
}

NIterator* SSDBImpl::nrscan(const Bytes &name,
		const Bytes &score_start, const Bytes &score_end, uint64_t limit)
{

	return niterator(this, name, score_start, score_end, limit, Iterator::BACKWARD);
}

static void get_nnames(Iterator *it, std::vector<std::string> *list){
	while(it->next()){
		Bytes ks = it->key();
		//dump(ks.data(), ks.size());
		if(ks.data()[0] != DataType::NSIZE){
			break;
		}
		std::string n;
		if(decode_nsize_key(ks, &n) == -1){
			continue;
		}
		list->push_back(n);
	}
}

int SSDBImpl::nlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
		std::vector<std::string> *list){
	std::string start;
	std::string end;
	
	start = encode_nsize_key(name_s);
	if(!name_e.empty()){
		end = encode_nsize_key(name_e);
	}
	
	Iterator *it = this->iterator(start, end, limit);
	get_nnames(it, list);
	delete it;
	return 0;
}

int SSDBImpl::nrlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
		std::vector<std::string> *list){
	std::string start;
	std::string end;

	start = encode_nsize_key(name_s);
	if(name_s.empty()){
		start.append(1, 255);
	}
	if(!name_e.empty()){
		end = encode_nsize_key(name_e);
	}

	Iterator *it = this->rev_iterator(start, end, limit);
	get_nnames(it, list);
	delete it;
	return 0;
}

static std::string filter_score(const Bytes &score){
	int64_t s = score.Int64();
	return str(s);
}

int SSDBImpl::nget(const Bytes &name, const Bytes &score, std::string *val){
	std::string buf = encode_nscore_key(name, score);
	leveldb::Status s = ldb->Get(leveldb::ReadOptions(), buf, val);
	if(s.IsNotFound()){
		return 0;
	}
	if(!s.ok()){
		log_error("nget error: %s", s.ToString().c_str());
		return -1;
	}
	return 1;
}

// returns the number of newly added items
static int nset_one(SSDBImpl *ssdb, const Bytes &name, const Bytes &val, const Bytes &score, char log_type){
	if(name.empty() || val.empty()){
		log_error("empty name or val!");
		return 0;
		//return -1;
	}
	if(name.size() > SSDB_KEY_LEN_MAX ){
		log_error("name too long!");
		return -1;
	}

	std::string new_score = filter_score(score);
	int ret = 0;
	std::string dbval;
	if(ssdb->nget(name, new_score, &dbval) == 0){ // not found
		std::string nkey = encode_nscore_key(name, score);
		ssdb->binlogs->Put(nkey, slice(val));
		ssdb->binlogs->add_log(log_type, BinlogCommand::NSET, nkey);
		ret = 1;
	}else{
		if(dbval != val){
			std::string nkey = encode_nscore_key(name, score);
			ssdb->binlogs->Put(nkey, slice(val));
			ssdb->binlogs->add_log(log_type, BinlogCommand::NSET, nkey);
			ret = 1;
		}else {
			ret = 0;
		}

	}
	return ret;
}

static int incr_nsize(SSDBImpl *ssdb, const Bytes &name, int64_t incr){
	int64_t size = ssdb->nsize(name);
	size += incr;
	std::string size_key = encode_nsize_key(name);
	if(size == 0){
		ssdb->binlogs->Delete(size_key);
	}else{
		ssdb->binlogs->Put(size_key, leveldb::Slice((char *)&size, sizeof(int64_t)));
	}
	return 0;
}
