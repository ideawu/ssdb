#include "binlog.h"
#include "util/log.h"
#include "util/strings.h"

#ifdef NDEBUG
	static const int LOG_QUEUE_SIZE  = 10 * 1000 * 1000;
#else
	static const int LOG_QUEUE_SIZE  = 10000;
#endif

/*
logs = new MemorySyncLogQueue(backlog);
p_logs = new PersistentSyncLogQueue(meta_db);

// load persistent sync logs into SyncLogQueue
if(p_logs->seq_min > 0){
	uint64_t seq = p_logs->seq_min;
	if(p_logs->seq_min + backlog < p_logs->seq_max){
		seq = p_logs->seq_max - backlog;
	}
	for(; seq<=p_logs->seq_max; seq++){
		Binlog log;
		int ret = p_logs->find(seq, &log);
		if(ret != 1){
			log_error("error loading sync logs from leveldb, seq: %llu", seq);
			break;
		}
		log_trace("load %llu, key: %s", seq, hexmem(log.key().data(), log.key().size()).c_str());
		logs->put(log);
	}
}
*/

/* Binlog */

Binlog::Binlog(uint64_t seq, char cmd, char type, const leveldb::Slice &key){
	buf.append((char *)(&seq), sizeof(uint64_t));
	buf.push_back(cmd);
	buf.push_back(type);
	buf.append(key.data(), key.size());
}

uint64_t Binlog::seq() const{
	return *((uint64_t *)(buf.data()));
}

char Binlog::type() const{
	return buf[sizeof(uint64_t)];
}

char Binlog::cmd() const{
	return buf[sizeof(uint64_t) + 1];
}

const Bytes Binlog::key() const{
	return Bytes(buf.data() + HEADER_LEN, buf.size() - HEADER_LEN);
}

int Binlog::load(const leveldb::Slice &s){
	if(s.size() < HEADER_LEN){
		return -1;
	}
	buf.assign(s.data(), s.size());
	return 0;
}

std::string Binlog::dumps() const{
	std::string str;
	if(buf.size() < HEADER_LEN){
		return str;
	}
	str.append("binlog# ");
	char buf[20];
	snprintf(buf, sizeof(buf), "%llu ", this->seq());
	str.append(buf);

	switch(this->type()){
		case BinlogType::NOOP:
			str.append("noop ");
			break;
		case BinlogType::SYNC:
			str.append("sync ");
			break;
		case BinlogType::MIRROR:
			str.append("mirror ");
			break;
		case BinlogType::DUMP:
			str.append("dump ");
			break;
	}
	switch(this->cmd()){
		case BinlogCommand::NONE:
			str.append("none ");
			break;
		case BinlogCommand::KSET:
			str.append("set ");
			break;
		case BinlogCommand::KDEL:
			str.append("del ");
			break;
		case BinlogCommand::HSET:
			str.append("hset ");
			break;
		case BinlogCommand::HDEL:
			str.append("hdel ");
			break;
		case BinlogCommand::ZSET:
			str.append("zset ");
			break;
		case BinlogCommand::ZDEL:
			str.append("zdel ");
			break;
		case BinlogCommand::BEGIN:
			str.append("begin ");
			break;
		case BinlogCommand::END:
			str.append("end ");
			break;
	}
	Bytes b = this->key();
	str.append(b.data(), b.size());
	return str;
}


/* SyncLogQueue */

static inline std::string encode_seq_key(uint64_t seq){
	seq = big_endian(seq);
	std::string ret;
	ret.push_back(DataType::SYNCLOG);
	ret.append((char *)&seq, sizeof(seq));
	return ret;
}

static inline uint64_t decode_seq_key(const leveldb::Slice &key){
	uint64_t seq = 0;
	if(key.size() == (sizeof(uint64_t) + 1) && key.data()[0] == DataType::SYNCLOG){
		seq = *((uint64_t *)(key.data() + 1));
		seq = big_endian(seq);
	}
	return seq;
}
	
BinlogQueue::BinlogQueue(leveldb::DB *db){
	this->db = db;
	this->min_seq = 0;
	this->last_seq = 0;
	this->tran_seq = 0;
	this->capacity = LOG_QUEUE_SIZE;
	
	Binlog log;
	if(this->find_last(&log) == 1){
		this->last_seq = log.seq();
	}
	if(this->find_next(1, &log) == 1){
		this->min_seq = log.seq();
	}
	log_debug("binlogs.capacity: %d, min_seq: %llu, last_seq: %llu", capacity, min_seq, last_seq);

	// TODO: start cleaning thread
	/*
	*/
	// TEST:
	uint64_t seq = this->min_seq;
	while(this->find_next(seq, &log) == 1){
		seq = log.seq() + 1;
		std::string s = log.dumps();
		log_trace("%s", s.c_str());
	}
}

BinlogQueue::~BinlogQueue(){
}

void BinlogQueue::begin(){
	tran_seq = last_seq;
	batch.Clear();
}

void BinlogQueue::rollback(){
	tran_seq = 0;
}

leveldb::Status BinlogQueue::commit(){
	leveldb::WriteOptions write_opts;
	leveldb::Status s = db->Write(write_opts, &batch);
	if(s.ok()){
		last_seq = tran_seq;
		tran_seq = 0;
	}
	return s;
}

void BinlogQueue::log(char type, char cmd, const leveldb::Slice &key){
	tran_seq ++;
	Binlog log(tran_seq, type, cmd, key);
	batch.Put(encode_seq_key(tran_seq), log.repr());
}

void BinlogQueue::log(char type, char cmd, const std::string &key){
	leveldb::Slice s(key);
	this->log(type, cmd, s);
}

// leveldb put
void BinlogQueue::Put(const leveldb::Slice& key, const leveldb::Slice& value){
	batch.Put(key, value);
}

// leveldb delete
void BinlogQueue::Delete(const leveldb::Slice& key){
	batch.Delete(key);
}
	
int BinlogQueue::find_next(uint64_t next_seq, Binlog *log) const{
	uint64_t ret = 0;
	std::string key_str = encode_seq_key(next_seq);
	leveldb::ReadOptions iterate_options;
	leveldb::Iterator *it = db->NewIterator(iterate_options);
	it->Seek(key_str);
	if(it->Valid()){
		leveldb::Slice key = it->key();
		if(decode_seq_key(key) != 0){
			leveldb::Slice val = it->value();
			if(log->load(val) == -1){
				ret = -1;
			}else{
				ret = 1;
			}
		}
	}
	delete it;
	return ret;
}

int BinlogQueue::find_last(Binlog *log) const{
	uint64_t ret = 0;
	std::string key_str = encode_seq_key(UINT64_MAX);
	leveldb::ReadOptions iterate_options;
	leveldb::Iterator *it = db->NewIterator(iterate_options);
	it->Seek(key_str);
	if(!it->Valid()){
		// Iterator::prev requires Valid, so we seek to last
		it->SeekToLast();
	}
	// UINT64_MAX is not used
	if(it->Valid()){
		it->Prev();
	}
	if(it->Valid()){
		leveldb::Slice key = it->key();
		if(decode_seq_key(key) != 0){
			leveldb::Slice val = it->value();
			if(log->load(val) == -1){
				ret = -1;
			}else{
				ret = 1;
			}
		}
	}
	delete it;
	return ret;
}
