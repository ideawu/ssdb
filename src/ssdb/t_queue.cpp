/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "t_queue.h"

static int qget_by_seq(leveldb::DB* db, const Bytes &name, uint64_t seq, std::string *val){
	std::string key = encode_qitem_key(name, seq);
	leveldb::Status s;

	s = db->Get(leveldb::ReadOptions(), key, val);
	if(s.IsNotFound()){
		return 0;
	}else if(!s.ok()){
		log_error("Get() error!");
		return -1;
	}else{
		return 1;
	}
}

static int qget_uint64(leveldb::DB* db, const Bytes &name, uint64_t seq, uint64_t *ret){
	std::string val;
	*ret = 0;
	int s = qget_by_seq(db, name, seq, &val);
	if(s == 1){
		if(val.size() != sizeof(uint64_t)){
			return -1;
		}
		memcpy(ret, val.data(), sizeof(uint64_t));
	}
	return s;
}

static int qdel_one(SSDBImpl *ssdb, const Bytes &name, uint64_t seq){
	std::string key = encode_qitem_key(name, seq);
	leveldb::Status s;

	ssdb->binlogs->Delete(key);
	return 0;
}

static int qset_one(SSDBImpl *ssdb, const Bytes &name, uint64_t seq, const Bytes &item){
	if(name.size() > SSDB_KEY_LEN_MAX ){
		log_error("name too long! %s", hexmem(name.data(), name.size()).c_str());
		return -1;
	}

	std::string key = encode_qitem_key(name, seq);
	leveldb::Status s;

	ssdb->binlogs->Put(key, slice(item));
	return 0;
}

static int64_t incr_qsize(SSDBImpl *ssdb, const Bytes &name, int64_t incr){
	int64_t size = ssdb->qsize(name);
	if(size == -1){
		return -1;
	}
	size += incr;
	if(size <= 0){
		ssdb->binlogs->Delete(encode_qsize_key(name));
		qdel_one(ssdb, name, QFRONT_SEQ);
		qdel_one(ssdb, name, QBACK_SEQ);
	}else{
		ssdb->binlogs->Put(encode_qsize_key(name), leveldb::Slice((char *)&size, sizeof(size)));
	}
	return size;
}

/****************/

int64_t SSDBImpl::qsize(const Bytes &name){
	std::string key = encode_qsize_key(name);
	std::string val;

	leveldb::Status s;
	s = ldb->Get(leveldb::ReadOptions(), key, &val);
	if(s.IsNotFound()){
		return 0;
	}else if(!s.ok()){
		log_error("Get() error!");
		return -1;
	}else{
		if(val.size() != sizeof(uint64_t)){
			return -1;
		}
		int64_t ret; 
		memcpy(&ret, val.data(), sizeof(int64_t));
		return ret;
	}
}

// @return 0: empty queue, 1: item peeked, -1: error
int SSDBImpl::qfront(const Bytes &name, std::string *item){
	int ret = 0;
	uint64_t seq;
	ret = qget_uint64(this->ldb, name, QFRONT_SEQ, &seq);
	if(ret == -1){
		return -1;
	}
	if(ret == 0){
		return 0;
	}
	ret = qget_by_seq(this->ldb, name, seq, item);
	return ret;
}

// @return 0: empty queue, 1: item peeked, -1: error
int SSDBImpl::qback(const Bytes &name, std::string *item){
	int ret = 0;
	uint64_t seq;
	ret = qget_uint64(this->ldb, name, QBACK_SEQ, &seq);
	if(ret == -1){
		return -1;
	}
	if(ret == 0){
		return 0;
	}
	ret = qget_by_seq(this->ldb, name, seq, item);
	return ret;
}

int SSDBImpl::qset_by_seq(const Bytes &name, uint64_t seq, const Bytes &item, char log_type){
	Transaction trans(binlogs);
	uint64_t min_seq, max_seq;
	int ret;
	int64_t size = this->qsize(name);
	if(size == -1){
		return -1;
	}
	ret = qget_uint64(this->ldb, name, QFRONT_SEQ, &min_seq);
	if(ret == -1){
		return -1;
	}
	max_seq = min_seq + size;
	if(seq < min_seq || seq > max_seq){
		return 0;
	}

	ret = qset_one(this, name, seq, item);
	if(ret == -1){
		return -1;
	}

	std::string buf = encode_qitem_key(name, seq);
	binlogs->add_log(log_type, BinlogCommand::QSET, buf);

	leveldb::Status s = binlogs->commit();
	if(!s.ok()){
		log_error("Write error!");
		return -1;
	}
	return 1;
}

// return: 0: index out of range, -1: error, 1: ok
int SSDBImpl::qset(const Bytes &name, int64_t index, const Bytes &item, char log_type){
	Transaction trans(binlogs);
	int64_t size = this->qsize(name);
	if(size == -1){
		return -1;
	}
	if(index >= size || index < -size){
		return 0;
	}
	
	int ret;
	uint64_t seq;
	if(index >= 0){
		ret = qget_uint64(this->ldb, name, QFRONT_SEQ, &seq);
		seq += index;
	}else{
		ret = qget_uint64(this->ldb, name, QBACK_SEQ, &seq);
		seq += index + 1;
	}
	if(ret == -1){
		return -1;
	}
	if(ret == 0){
		return 0;
	}

	ret = qset_one(this, name, seq, item);
	if(ret == -1){
		return -1;
	}

	//log_info("qset %s %" PRIu64 "", hexmem(name.data(), name.size()).c_str(), seq);
	std::string buf = encode_qitem_key(name, seq);
	binlogs->add_log(log_type, BinlogCommand::QSET, buf);
	
	leveldb::Status s = binlogs->commit();
	if(!s.ok()){
		log_error("Write error!");
		return -1;
	}
	return 1;
}

int64_t SSDBImpl::_qpush(const Bytes &name, const Bytes &item, uint64_t front_or_back_seq, char log_type){
	Transaction trans(binlogs);

	int ret;
	// generate seq
	uint64_t seq;
	ret = qget_uint64(this->ldb, name, front_or_back_seq, &seq);
	if(ret == -1){
		return -1;
	}
	// update front and/or back
	if(ret == 0){
		seq = QITEM_SEQ_INIT;
		ret = qset_one(this, name, QFRONT_SEQ, Bytes(&seq, sizeof(seq)));
		if(ret == -1){
			return -1;
		}
		ret = qset_one(this, name, QBACK_SEQ, Bytes(&seq, sizeof(seq)));
	}else{
		seq += (front_or_back_seq == QFRONT_SEQ)? -1 : +1;
		ret = qset_one(this, name, front_or_back_seq, Bytes(&seq, sizeof(seq)));
	}
	if(ret == -1){
		return -1;
	}
	if(seq <= QITEM_MIN_SEQ || seq >= QITEM_MAX_SEQ){
		log_info("queue is full, seq: %" PRIu64 " out of range", seq);
		return -1;
	}
	
	// prepend/append item
	ret = qset_one(this, name, seq, item);
	if(ret == -1){
		return -1;
	}

	std::string buf = encode_qitem_key(name, seq);
	if(front_or_back_seq == QFRONT_SEQ){
		binlogs->add_log(log_type, BinlogCommand::QPUSH_FRONT, buf);
	}else{
		binlogs->add_log(log_type, BinlogCommand::QPUSH_BACK, buf);
	}
	
	// update size
	int64_t size = incr_qsize(this, name, +1);
	if(size == -1){
		return -1;
	}

	leveldb::Status s = binlogs->commit();
	if(!s.ok()){
		log_error("Write error! %s", s.ToString().c_str());
		return -1;
	}
	return size;
}

int64_t SSDBImpl::qpush_front(const Bytes &name, const Bytes &item, char log_type){
	return _qpush(name, item, QFRONT_SEQ, log_type);
}

int64_t SSDBImpl::qpush_back(const Bytes &name, const Bytes &item, char log_type){
	return _qpush(name, item, QBACK_SEQ, log_type);
}

int SSDBImpl::_qpop(const Bytes &name, std::string *item, uint64_t front_or_back_seq, char log_type){
	Transaction trans(binlogs);
	
	int ret;
	uint64_t seq;
	ret = qget_uint64(this->ldb, name, front_or_back_seq, &seq);
	if(ret == -1){
		return -1;
	}
	if(ret == 0){
		return 0;
	}
	
	ret = qget_by_seq(this->ldb, name, seq, item);
	if(ret == -1){
		return -1;
	}
	if(ret == 0){
		return 0;
	}

	// delete item
	ret = qdel_one(this, name, seq);
	if(ret == -1){
		return -1;
	}

	if(front_or_back_seq == QFRONT_SEQ){
		binlogs->add_log(log_type, BinlogCommand::QPOP_FRONT, name.String());
	}else{
		binlogs->add_log(log_type, BinlogCommand::QPOP_BACK, name.String());
	}

	// update size
	int64_t size = incr_qsize(this, name, -1);
	if(size == -1){
		return -1;
	}
		
	// update front
	if(size > 0){
		seq += (front_or_back_seq == QFRONT_SEQ)? +1 : -1;
		//log_debug("seq: %" PRIu64 ", ret: %d", seq, ret);
		ret = qset_one(this, name, front_or_back_seq, Bytes(&seq, sizeof(seq)));
		if(ret == -1){
			return -1;
		}
	}
		
	leveldb::Status s = binlogs->commit();
	if(!s.ok()){
		log_error("Write error! %s", s.ToString().c_str());
		return -1;
	}
	return 1;
}

// @return 0: empty queue, 1: item popped, -1: error
int SSDBImpl::qpop_front(const Bytes &name, std::string *item, char log_type){
	return _qpop(name, item, QFRONT_SEQ, log_type);
}

int SSDBImpl::qpop_back(const Bytes &name, std::string *item, char log_type){
	return _qpop(name, item, QBACK_SEQ, log_type);
}

static void get_qnames(Iterator *it, std::vector<std::string> *list){
	while(it->next()){
		Bytes ks = it->key();
		//dump(ks.data(), ks.size());
		if(ks.data()[0] != DataType::QSIZE){
			break;
		}
		std::string n;
		if(decode_qsize_key(ks, &n) == -1){
			continue;
		}
		list->push_back(n);
	}
}

int SSDBImpl::qlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
		std::vector<std::string> *list){
	std::string start;
	std::string end;
	
	start = encode_qsize_key(name_s);
	if(!name_e.empty()){
		end = encode_qsize_key(name_e);
	}
	
	Iterator *it = this->iterator(start, end, limit);
	get_qnames(it, list);
	delete it;
	return 0;
}

int SSDBImpl::qrlist(const Bytes &name_s, const Bytes &name_e, uint64_t limit,
		std::vector<std::string> *list){
	std::string start;
	std::string end;
	
	start = encode_qsize_key(name_s);
	if(name_s.empty()){
		start.append(1, 255);
	}
	if(!name_e.empty()){
		end = encode_qsize_key(name_e);
	}
	
	Iterator *it = this->rev_iterator(start, end, limit);
	get_qnames(it, list);
	delete it;
	return 0;
}

int SSDBImpl::qfix(const Bytes &name){
	Transaction trans(binlogs);
	std::string key_s = encode_qitem_key(name, QITEM_MIN_SEQ - 1);
	std::string key_e = encode_qitem_key(name, QITEM_MAX_SEQ);

	bool error = false;
	uint64_t seq_min = 0;
	uint64_t seq_max = 0;
	uint64_t count = 0;
	Iterator *it = this->iterator(key_s, key_e, QITEM_MAX_SEQ);
	while(it->next()){
		//dump(it->key().data(), it->key().size());
		if(seq_min == 0){
			if(decode_qitem_key(it->key(), NULL, &seq_min) == -1){
				// or just delete it?
				error = true;
				break;
			}
		}
		if(decode_qitem_key(it->key(), NULL, &seq_max) == -1){
			error = true;
			break;
		}
		count ++;
	}
	delete it;
	if(error){
		return -1;
	}
	
	if(count == 0){
		this->binlogs->Delete(encode_qsize_key(name));
		qdel_one(this, name, QFRONT_SEQ);
		qdel_one(this, name, QBACK_SEQ);
	}else{
		this->binlogs->Put(encode_qsize_key(name), leveldb::Slice((char *)&count, sizeof(count)));
		qset_one(this, name, QFRONT_SEQ, Bytes(&seq_min, sizeof(seq_min)));
		qset_one(this, name, QBACK_SEQ, Bytes(&seq_max, sizeof(seq_max)));
	}
		
	leveldb::Status s = binlogs->commit();
	if(!s.ok()){
		log_error("Write error!");
		return -1;
	}
	return 0;
}

int SSDBImpl::qslice(const Bytes &name, int64_t begin, int64_t end,
		std::vector<std::string> *list)
{
	int ret;
	uint64_t seq_begin, seq_end;
	if(begin >= 0 && end >= 0){
		uint64_t tmp_seq;
		ret = qget_uint64(this->ldb, name, QFRONT_SEQ, &tmp_seq);
		if(ret != 1){
			return ret;
		}
		seq_begin = tmp_seq + begin;
		seq_end = tmp_seq + end;
	}else if(begin < 0 && end < 0){
		uint64_t tmp_seq;
		ret = qget_uint64(this->ldb, name, QBACK_SEQ, &tmp_seq);
		if(ret != 1){
			return ret;
		}
		seq_begin = tmp_seq + begin + 1;
		seq_end = tmp_seq + end + 1;
	}else{
		uint64_t f_seq, b_seq;
		ret = qget_uint64(this->ldb, name, QFRONT_SEQ, &f_seq);
		if(ret != 1){
			return ret;
		}
		ret = qget_uint64(this->ldb, name, QBACK_SEQ, &b_seq);
		if(ret != 1){
			return ret;
		}
		if(begin >= 0){
			seq_begin = f_seq + begin;
		}else{
			seq_begin = b_seq + begin + 1;
		}
		if(end >= 0){
			seq_end = f_seq + end;
		}else{
			seq_end = b_seq + end + 1;
		}
	}
	
	for(; seq_begin <= seq_end; seq_begin++){
		std::string item;
		ret = qget_by_seq(this->ldb, name, seq_begin, &item);
		if(ret == -1){
			return -1;
		}
		if(ret == 0){
			return 0;
		}
		list->push_back(item);
	}
	return 0;
}

int SSDBImpl::qget(const Bytes &name, int64_t index, std::string *item){
	int ret;
	uint64_t seq;
	if(index >= 0){
		ret = qget_uint64(this->ldb, name, QFRONT_SEQ, &seq);
		seq += index;
	}else{
		ret = qget_uint64(this->ldb, name, QBACK_SEQ, &seq);
		seq += index + 1;
	}
	if(ret == -1){
		return -1;
	}
	if(ret == 0){
		return 0;
	}
	
	ret = qget_by_seq(this->ldb, name, seq, item);
	return ret;
}
