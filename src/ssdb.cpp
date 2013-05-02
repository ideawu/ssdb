#include "ssdb.h"
#include "slave.h"
#include "leveldb/iterator.h"
#include "leveldb/cache.h"
#include "leveldb/filter_policy.h"

SSDB::SSDB(){
	db = NULL;
	meta_db = NULL;
	binlogs = NULL;
}

SSDB::~SSDB(){
	for(std::vector<Slave *>::iterator it = slaves.begin(); it != slaves.end(); it++){
		Slave *slave = *it;
		slave->stop();
		delete slave;
	}
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

	log_info("main_db      : %s", main_db_path.c_str());
	log_info("meta_db      : %s", meta_db_path.c_str());
	log_info("cache_size   : %d MB", cache_size);
	log_info("block_size   : %d KB", block_size);
	log_info("write_buffer : %d MB", write_buffer_size);

	SSDB *ssdb = new SSDB();
	//
	ssdb->options.create_if_missing = true;
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

	status = leveldb::DB::Open(ssdb->options, main_db_path, &ssdb->db);
	if(!status.ok()){
		log_error("open main_db failed");
		goto err;
	}
	ssdb->binlogs = new BinlogQueue(ssdb->db);

	{ // slaves
		const Config *repl_conf = conf.get("replication");
		if(repl_conf != NULL){
			std::vector<Config *> children = repl_conf->children;
			for(std::vector<Config *>::iterator it = children.begin(); it != children.end(); it++){
				Config *c = *it;
				if(c->key != "slaveof"){
					continue;
				}
				std::string ip = c->get_str("ip");
				int port = c->get_num("port");
				if(ip == "" || port <= 0 || port > 65535){
					continue;
				}
				bool is_mirror = false;
				std::string type = c->get_str("type");
				if(type == "mirror"){
					is_mirror = true;
				}else{
					type = "sync";
					is_mirror = false;
				}
				
				log_info("slaveof: %s:%d, type: %s", ip.c_str(), port, type.c_str());
				Slave *slave = new Slave(ssdb, ssdb->meta_db, ip.c_str(), port, is_mirror);
				slave->start();
				ssdb->slaves.push_back(slave);
			}
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
	leveldb::ReadOptions iterate_options;
	iterate_options.fill_cache = false;
	it = db->NewIterator(iterate_options);
	it->Seek(start);
	if(it->Valid() && it->key() == start){
		it->Next();
	}
	return new Iterator(it, end, limit);
}

Iterator* SSDB::rev_iterator(const std::string &start, const std::string &end, int limit) const{
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
	if(it->Valid() && it->key() == start){
		it->Prev();
	}
	return new Iterator(it, end, limit, Iterator::BACKWARD);
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

/*
int SSDB::key_range(char data_type, std::string *start, std::string *end) const{
	leveldb::ReadOptions iterate_options;
	leveldb::Iterator *it = db->NewIterator(iterate_options);

	std::string start_str;
	start_str.push_back(data_type);
	
	it->Seek(key_str);
	if(!it->Valid()){
		// Iterator::prev requires Valid, so we seek to last
		it->SeekToLast();
	}
	// UINT64_MAX is not used
	if(it->Valid()){
		it->Prev();
	}
	std::string ret;
	if(it->Valid()){
		leveldb::Slice key = it->key();
		ret.assign(key.data(), key.size());
	}
	delete it;
	return ret;
}
*/
