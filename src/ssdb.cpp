#include <stdlib.h>
#include "ssdb.h"
#include "slave.h"
#include "leveldb/iterator.h"
#include "leveldb/cache.h"
#include "leveldb/filter_policy.h"

SSDB::SSDB(){
	db = NULL;
	meta_db = NULL;
	slave = NULL;
	replication = NULL;
	iterate_options.fill_cache = false;
	options.create_if_missing = true;
}

SSDB::~SSDB(){
	if(slave){
		delete slave;
	}
	if(db){
		delete db;
	}
	if(replication){
		delete replication;
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
}

SSDB* SSDB::open(const Config &conf, const std::string &base_dir){
	std::string main_db_path = base_dir + "/data";
	std::string meta_db_path = base_dir + "/meta";
	std::string role = conf.get_str("replication.role");
	int cache_size = conf.get_num("leveldb.cache_size");
	int write_buffer_size = conf.get_num("leveldb.write_buffer_size");
	int block_size = conf.get_num("leveldb.block_size");

	if(cache_size <= 0){
		cache_size = 8;
	}
	if(write_buffer_size <= 0){
		write_buffer_size = 4;
	}
	if(block_size <= 0){
		block_size = 4;
	}
	// work as master if not implicitly configured as slave
	role = role == "slave"? "slave" : "master";

	log_info("role       : %s", role.c_str());
	log_info("main_db    : %s", main_db_path.c_str());
	log_info("meta_db    : %s", meta_db_path.c_str());
	log_info("cache_size : %d MB", cache_size);
	log_info("block_size : %d KB", block_size);
	log_info("write_buffer_size : %d MB", write_buffer_size);

	SSDB *ssdb = new SSDB();
	//
	ssdb->options.filter_policy = leveldb::NewBloomFilterPolicy(10);
	ssdb->options.block_cache = leveldb::NewLRUCache(cache_size * 1048576);
	ssdb->options.block_size = block_size * 1024;
	ssdb->options.write_buffer_size = write_buffer_size * 1024 * 1024;

	leveldb::Status status;
	{
		leveldb::Options options;
		options.create_if_missing = true;
		status = leveldb::DB::Open(options, meta_db_path, &ssdb->meta_db);
		if(!status.ok()){
			goto err;
		}
	}

	if(role == "master"){
		MyReplication *repl = new MyReplication(ssdb->meta_db);
		ssdb->replication = repl;
		ssdb->options.replication = repl;
	}

	status = leveldb::DB::Open(ssdb->options, main_db_path, &ssdb->db);
	if(!status.ok()){
		log_error("open main_db failed");
		goto err;
	}

	// a master can also be a slave
	{ // slave
		std::string ip;
		int port;
		ip = conf.get_str("replication.slaveof.ip");
		port = conf.get_num("replication.slaveof.port");
		if(ip != ""){
			ssdb->slave = new Slave(ssdb, ssdb->meta_db, ip.c_str(), port);
			ssdb->slave->start();
			log_info("slaveof: %s:%d", ip.c_str(), port);
		}
	}

	return ssdb;
err:
	if(ssdb){
		delete ssdb;
	}
	return NULL;
}

Iterator* SSDB::iterator(const std::string &start, const std::string &end, int limit) const{
	leveldb::Iterator *it;
	it = db->NewIterator(iterate_options);
	it->Seek(start);
	if(it->Valid() && it->key() == start){
		it->Next();
	}
	return new Iterator(it, end, limit);
}

Iterator* SSDB::rev_iterator(const std::string &start, const std::string &end, int limit) const{
	leveldb::Iterator *it;
	it = db->NewIterator(iterate_options);
	it->Seek(start);
	if(!it->Valid()){
		it->SeekToLast();
	}else{
		it->Prev();
	}
	if(it->Valid() && it->key() == start){
		it->Prev();
	}
	return new Iterator(it, end, limit, Iterator::BACKWARD);
}


/* raw operates */

int SSDB::raw_set(const Bytes &key, const Bytes &val) const{
	leveldb::Status s = db->Put(write_options, key.Slice(), val.Slice());
	if(!s.ok()){
		log_error("set error: %s", s.ToString().c_str());
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

int SSDB::raw_del(const Bytes &key) const{
	leveldb::Status s = db->Delete(write_options, key.Slice());
	if(!s.ok()){
		log_error("del error: %s", s.ToString().c_str());
		return -1;
	}
	return 1;
}
