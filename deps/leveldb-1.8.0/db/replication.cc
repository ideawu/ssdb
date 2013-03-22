#include "db/dbformat.h"
#include "db/write_batch_internal.h"
#include "leveldb/replication.h"

namespace leveldb{

namespace {
	class Handler : public WriteBatch::Handler {
		public:
			bool is_mirror;
			Replication *repl_;
			SequenceNumber sequence_;

			void Put(const leveldb::Slice& key, const leveldb::Slice& value) {
				repl_->Put(sequence_, key, value, is_mirror);
				sequence_++;
			}
			void Delete(const leveldb::Slice& key) {
				repl_->Delete(sequence_, key, is_mirror);
				sequence_++;
			}
	};
}

void Replication::Write(const leveldb::WriteBatch* batch, bool is_mirror){
	Handler handler;
	handler.repl_ = this;
	handler.is_mirror  = is_mirror;
	handler.sequence_ = WriteBatchInternal::Sequence(batch);
	batch->Iterate(&handler);
}

}
