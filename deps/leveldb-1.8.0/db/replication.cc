#include "db/dbformat.h"
#include "db/write_batch_internal.h"
#include "leveldb/replication.h"

namespace leveldb{

namespace {
	class Handler : public WriteBatch::Handler {
		public:
			Replication *repl_;
			SequenceNumber sequence_;

			void Put(const leveldb::Slice& key, const leveldb::Slice& value) {
				repl_->Put(sequence_, key, value);
				sequence_++;
			}
			void Delete(const leveldb::Slice& key) {
				repl_->Delete(sequence_, key);
				sequence_++;
			}
	};
}

void Replication::Write(const leveldb::WriteBatch* batch){
	Handler handler;
	handler.repl_ = this;
	handler.sequence_ = WriteBatchInternal::Sequence(batch);
	batch->Iterate(&handler);
}

}
