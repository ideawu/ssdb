#ifndef SSDB_REPL_H_
#define SSDB_REPL_H_

#include <stdint.h>
#include <string>
#include "leveldb/db.h"
#include "leveldb/options.h"
#include "leveldb/slice.h"
#include "leveldb/replication.h"
#include "util/lock.h"
#include "util/bytes.h"


class Synclog{
	public:
		static const char SET  = 's';
		static const char DEL  = 'd';
		static const char MIRROR_SET  = 'S';
		static const char MIRROR_DEL  = 'D';
	private:
		std::string buf;
		static const int KEY_POS = sizeof(uint64_t) + 1;
	public:
		static const int MIN_SIZE = sizeof(uint64_t) + 1;

		Synclog(){}
		Synclog(std::string &s){
			buf = s;
		}
		Synclog(uint64_t seq, char type);
		Synclog(uint64_t seq, char type, const leveldb::Slice &key);

		uint64_t seq() const;
		char type() const;
		const Bytes key() const;

		const char* data() const{
			return buf.data();
		}
		int size() const{
			return (int)buf.size();
		}
};

// circular queue
class SyncLogQueue{
	protected:
		Mutex mutex;
		int start;
		int size;

		int index(int offset) const{
			int ret = start + offset;
			return ret < total? ret : (ret - total);
		}
		virtual int find_by_pos(int pos, Synclog *log) = 0;
	public:
		int total;
		uint64_t seq_min;
		uint64_t seq_max;
		virtual void put(const Synclog &log) = 0;
		/** @returns
		 1 : log.seq greater than or equal to seq
		 0 : not found
		 -1: error
		 */
		virtual int find(uint64_t seq, Synclog *log);
		virtual int find_last(Synclog *log);
};

class MemorySyncLogQueue : public SyncLogQueue{
	private:
		Synclog *ptr; // array of Synclog
	protected:
		int find_by_pos(int pos, Synclog *log);
	public:
		MemorySyncLogQueue();
		~MemorySyncLogQueue();
		void put(const Synclog &log);
};


class PersistentSyncLogQueue : public SyncLogQueue{
	private:
		leveldb::DB* meta_db;

		uint64_t find_seq_at_pos(int pos);
		int find_most_greater_seq_pos(uint64_t low_seq, int spos, int epos);
	protected:
		int find_by_pos(int pos, Synclog *log);
	public:
		PersistentSyncLogQueue(leveldb::DB *db);
		~PersistentSyncLogQueue();
		void put(const Synclog &log);
};


class MyReplication : public leveldb::Replication{
	private:
		int backlog;
	public:
		MyReplication(leveldb::DB *meta_db);
		~MyReplication();

		PersistentSyncLogQueue *logs;

		void Put(uint64_t seq, const leveldb::Slice &key, const leveldb::Slice &val, bool mirror);
		void Delete(uint64_t seq, const leveldb::Slice &key, bool mirror);
};

#endif
