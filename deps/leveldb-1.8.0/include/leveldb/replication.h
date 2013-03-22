#ifndef STORAGE_LEVELDB_INCLUDE_REPLICATION_H_
#define STORAGE_LEVELDB_INCLUDE_REPLICATION_H_

#include <string>
#include "leveldb/status.h"
#include "leveldb/write_batch.h"

namespace leveldb {

class Replication{
	public:
		// if mirror == true, the operation will be sync to all slaves,
		// but not mirror servers.
		void Write(const leveldb::WriteBatch* batch, bool mirror);
		virtual void Put(uint64_t seq, const leveldb::Slice& key, const leveldb::Slice& value, bool mirror=false)=0;
		virtual void Delete(uint64_t seq, const leveldb::Slice& key, bool mirror=false)=0;
};

}  // namespace leveldb

#endif
