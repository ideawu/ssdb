#ifndef STORAGE_LEVELDB_INCLUDE_REPLICATION_H_
#define STORAGE_LEVELDB_INCLUDE_REPLICATION_H_

#include <string>
#include "leveldb/status.h"
#include "leveldb/write_batch.h"

namespace leveldb {

class Replication{
	public:
		void Write(const leveldb::WriteBatch* batch);
		virtual void Put(uint64_t seq, const leveldb::Slice& key, const leveldb::Slice& value)=0;
		virtual void Delete(uint64_t seq, const leveldb::Slice& key)=0;
};

}  // namespace leveldb

#endif
