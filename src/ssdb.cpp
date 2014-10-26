#include "ssdb.h"
#include "slave.h"
#include "leveldb/env.h"
#include "leveldb/iterator.h"
#include "leveldb/cache.h"
#include "leveldb/filter_policy.h"

#include "t_kv.h"
#include "t_hash.h"
#include "t_zset.h"
#include "t_queue.h"

SSDB::SSDB(): sync_speed_(0){
	db = NULL;
	meta_db = NULL;
	binlogs = NULL;
}

SSDB::~SSDB(){
	if(binlogs){
		delete binlogs;
	}
	if(db){
		delete db;
	}
	if(options.block_cache){
		delete options.block_cache;
	}
	if(options.filter_policy){
		delete options.filter_policy;
	}
	if(meta_db){
		delete meta_db;
	}
	log_debug("SSDB finalized");
}

SSDB* SSDB::open(const Config &conf, const std::string &base_dir){
	std::string main_db_path = base_dir + "/data";
	std::string meta_db_path = base_dir + "/meta";
	size_t cache_size = (size_t)conf.get_num("leveldb.cache_size");
	int max_open_files = conf.get_num("leveldb.max_open_files");
	int write_buffer_size = conf.get_num("leveldb.write_buffer_size");
	int block_size = conf.get_num("leveldb.block_size");
	int compaction_speed = conf.get_num("leveldb.compaction_speed");
	std::string compression = conf.get_str("leveldb.compression");
	std::string binlog_onoff = conf.get_str("replication.binlog");
	int sync_speed = conf.get_num("replication.sync_speed");

	strtolower(&compression);
	if(compression != "yes"){
		compression = "no";
	}
	strtolower(&binlog_onoff);
	if(binlog_onoff != "no"){
		binlog_onoff = "yes";
	}

	if(cache_size <= 0){
		cache_size = 8;
	}
	if(write_buffer_size <= 0){
		write_buffer_size = 4;
	}
	if(block_size <= 0){
		block_size = 4;
	}
	if(max_open_files <= 0){
		max_open_files = cache_size / 1024 * 30;
		if(max_open_files < 100){
			max_open_files = 100;
		}
		if(max_open_files > 1000){
			max_open_files = 1000;
		}
	}

	log_info("main_db          : %s", main_db_path.c_str());
	log_info("meta_db          : %s", meta_db_path.c_str());
	log_info("cache_size       : %d MB", cache_size);
	log_info("block_size       : %d KB", block_size);
	log_info("write_buffer     : %d MB", write_buffer_size);
	log_info("compaction_speed : %d MB/s", compaction_speed);
	log_info("sync_speed       : %d MB/s", sync_speed);
	log_info("compression      : %s", compression.c_str());
	log_info("binlog           : %s", binlog_onoff.c_str());
	log_info("max_open_files   : %d", max_open_files);

	SSDB *ssdb = new SSDB();
	ssdb->sync_speed_ = sync_speed;
	//
	ssdb->options.max_open_files = max_open_files;
	ssdb->options.create_if_missing = true;
	ssdb->options.filter_policy = leveldb::NewBloomFilterPolicy(10);
	ssdb->options.block_cache = leveldb::NewLRUCache(cache_size * 1048576);
	ssdb->options.block_size = block_size * 1024;
	ssdb->options.write_buffer_size = write_buffer_size * 1024 * 1024;
	ssdb->options.compaction_speed = compaction_speed;
	if(compression == "yes"){
		ssdb->options.compression = leveldb::kSnappyCompression;
	}else{
		ssdb->options.compression = leveldb::kNoCompression;
	}

	leveldb::Status status;
	{
		leveldb::Options options;
		options.create_if_missing = true;
		status = leveldb::DB::Open(options, meta_db_path, &ssdb->meta_db);
		if(!status.ok()){
			log_error("open meta_db failed!");
			goto err;
		}
	}
	
	// load kv_range
	{
		int ret = ssdb->get_kv_range(&ssdb->kv_range_s, &ssdb->kv_range_e);
		if(ret == -1){
			log_error("load key_range failed!");
			goto err;
		}
		log_info("key_range.kv    : \"%s\", \"%s\"",
			str_escape(ssdb->kv_range_s).c_str(),
			str_escape(ssdb->kv_range_e).c_str()
			);
	}

	status = leveldb::DB::Open(ssdb->options, main_db_path, &ssdb->db);
	if(!status.ok()){
		log_error("open main_db failed");
		goto err;
	}
	ssdb->binlogs = new BinlogQueue(ssdb->db);
	if(binlog_onoff == "no"){
		ssdb->binlogs->no_log();
	}

	return ssdb;
err:
	if(ssdb){
		delete ssdb;
	}
	return NULL;
}

Iterator* SSDB::iterator(const std::string &start, const std::string &end, uint64_t limit) const{
	leveldb::Iterator *it;
	leveldb::ReadOptions iterate_options;
	iterate_options.fill_cache = false;
	it = db->NewIterator(iterate_options);
	it->Seek(start);
	if(it->Valid() && it->key() == start){
		it->Next();
	}
	return new Iterator(it, end, limit);
}

Iterator* SSDB::rev_iterator(const std::string &start, const std::string &end, uint64_t limit) const{
	leveldb::Iterator *it;
	leveldb::ReadOptions iterate_options;
	iterate_options.fill_cache = false;
	it = db->NewIterator(iterate_options);
	it->Seek(start);
	if(!it->Valid()){
		it->SeekToLast();
	}else{
		it->Prev();
	}
	return new Iterator(it, end, limit, Iterator::BACKWARD);
}

	
int SSDB::set_kv_range(const std::string &start, const std::string &end){
	leveldb::WriteBatch batch;
	batch.Put("kv_range_s", start);
	batch.Put("kv_range_e", end);
	leveldb::Status s = meta_db->Write(leveldb::WriteOptions(), &batch);
	if(!s.ok()){
		return -1;
	}
	kv_range_s = start;
	kv_range_e = end;
	return 0;
}

int SSDB::get_kv_range(std::string *start, std::string *end){
	leveldb::Status s;
	s = meta_db->Get(leveldb::ReadOptions(), "kv_range_s", start);
	if(!s.ok() && !s.IsNotFound()){
		return -1;
	}
	s = meta_db->Get(leveldb::ReadOptions(), "kv_range_e", end);
	if(!s.ok() && !s.IsNotFound()){
		return -1;
	}
	return 0;
}

bool SSDB::in_kv_range(const Bytes &key) const{
	if((this->kv_range_s.size() && this->kv_range_s >= key)
		|| (this->kv_range_e.size() && this->kv_range_e < key))
	{
		return false;
	}
	return true;
}

bool SSDB::in_kv_range(const std::string &key) const{
	if((this->kv_range_s.size() && this->kv_range_s >= key)
		|| (this->kv_range_e.size() && this->kv_range_e < key))
	{
		return false;
	}
	return true;
}


/* raw operates */

int SSDB::raw_set(const Bytes &key, const Bytes &val) const{
	leveldb::WriteOptions write_opts;
	leveldb::Status s = db->Put(write_opts, key.Slice(), val.Slice());
	if(!s.ok()){
		log_error("set error: %s", s.ToString().c_str());
		return -1;
	}
	return 1;
}

int SSDB::raw_del(const Bytes &key) const{
	leveldb::WriteOptions write_opts;
	leveldb::Status s = db->Delete(write_opts, key.Slice());
	if(!s.ok()){
		log_error("del error: %s", s.ToString().c_str());
		return -1;
	}
	return 1;
}

int SSDB::raw_get(const Bytes &key, std::string *val) const{
	leveldb::ReadOptions opts;
	opts.fill_cache = false;
	leveldb::Status s = db->Get(opts, key.Slice(), val);
	if(s.IsNotFound()){
		return 0;
	}
	if(!s.ok()){
		log_error("get error: %s", s.ToString().c_str());
		return -1;
	}
	return 1;
}

std::vector<std::string> SSDB::info() const{
	//  "leveldb.num-files-at-level<N>" - return the number of files at level <N>,
	//     where <N> is an ASCII representation of a level number (e.g. "0").
	//  "leveldb.stats" - returns a multi-line string that describes statistics
	//     about the internal operation of the DB.
	//  "leveldb.sstables" - returns a multi-line string that describes all
	//     of the sstables that make up the db contents.
	std::vector<std::string> info;
	std::vector<std::string> keys;
	/*
	for(int i=0; i<7; i++){
		char buf[128];
		snprintf(buf, sizeof(buf), "leveldb.num-files-at-level%d", i);
		keys.push_back(buf);
	}
	*/
	keys.push_back("leveldb.stats");
	//keys.push_back("leveldb.sstables");

	for(size_t i=0; i<keys.size(); i++){
		std::string key = keys[i];
		std::string val;
		if(db->GetProperty(key, &val)){
			info.push_back(key);
			info.push_back(val);
		}
	}

	return info;
}

void SSDB::compact() const{
	db->CompactRange(NULL, NULL);
}

int SSDB::key_range(std::vector<std::string> *keys) const{
	int ret = 0;
	std::string kstart, kend;
	std::string hstart, hend;
	std::string zstart, zend;
	std::string qstart, qend;
	
	Iterator *it;
	
	it = this->iterator(encode_kv_key(""), "", 1);
	if(it->next()){
		Bytes ks = it->key();
		if(ks.data()[0] == DataType::KV){
			std::string n;
			if(decode_kv_key(ks, &n) == -1){
				ret = -1;
			}else{
				kstart = n;
			}
		}
	}
	delete it;
	
	it = this->rev_iterator(encode_kv_key("\xff"), "", 1);
	if(it->next()){
		Bytes ks = it->key();
		if(ks.data()[0] == DataType::KV){
			std::string n;
			if(decode_kv_key(ks, &n) == -1){
				ret = -1;
			}else{
				kend = n;
			}
		}
	}
	delete it;
	
	it = this->iterator(encode_hsize_key(""), "", 1);
	if(it->next()){
		Bytes ks = it->key();
		if(ks.data()[0] == DataType::HSIZE){
			std::string n;
			if(decode_hsize_key(ks, &n) == -1){
				ret = -1;
			}else{
				hstart = n;
			}
		}
	}
	delete it;
	
	it = this->rev_iterator(encode_hsize_key("\xff"), "", 1);
	if(it->next()){
		Bytes ks = it->key();
		if(ks.data()[0] == DataType::HSIZE){
			std::string n;
			if(decode_hsize_key(ks, &n) == -1){
				ret = -1;
			}else{
				hend = n;
			}
		}
	}
	delete it;
	
	it = this->iterator(encode_zsize_key(""), "", 1);
	if(it->next()){
		Bytes ks = it->key();
		if(ks.data()[0] == DataType::ZSIZE){
			std::string n;
			if(decode_hsize_key(ks, &n) == -1){
				ret = -1;
			}else{
				zstart = n;
			}
		}
	}
	delete it;
	
	it = this->rev_iterator(encode_zsize_key("\xff"), "", 1);
	if(it->next()){
		Bytes ks = it->key();
		if(ks.data()[0] == DataType::ZSIZE){
			std::string n;
			if(decode_hsize_key(ks, &n) == -1){
				ret = -1;
			}else{
				zend = n;
			}
		}
	}
	delete it;
	
	it = this->iterator(encode_qsize_key(""), "", 1);
	if(it->next()){
		Bytes ks = it->key();
		if(ks.data()[0] == DataType::QSIZE){
			std::string n;
			if(decode_qsize_key(ks, &n) == -1){
				ret = -1;
			}else{
				qstart = n;
			}
		}
	}
	delete it;
	
	it = this->rev_iterator(encode_qsize_key("\xff"), "", 1);
	if(it->next()){
		Bytes ks = it->key();
		if(ks.data()[0] == DataType::QSIZE){
			std::string n;
			if(decode_qsize_key(ks, &n) == -1){
				ret = -1;
			}else{
				qend = n;
			}
		}
	}
	delete it;

	keys->push_back(kstart);
	keys->push_back(kend);
	keys->push_back(hstart);
	keys->push_back(hend);
	keys->push_back(zstart);
	keys->push_back(zend);
	keys->push_back(qstart);
	keys->push_back(qend);
	
	return ret;
}
