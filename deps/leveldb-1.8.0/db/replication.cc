#include "db/dbformat.h"
#include "db/write_batch_internal.h"
#include "leveldb/replication.h"

namespace leveldb{

namespace {
	class Handler : public WriteBatch::Handler {
		public:
			bool noop;
			Replication *repl_;
			SequenceNumber sequence_;

			void Put(const leveldb::Slice& key, const leveldb::Slice& value) {
				if(noop){
					repl_->Noop(sequence_);
				}else{
					repl_->Put(sequence_, key, value);
				}
				sequence_++;
			}
			void Delete(const leveldb::Slice& key) {
				if(noop){
					repl_->Noop(sequence_);
				}else{
					repl_->Delete(sequence_, key);
				}
				sequence_++;
			}
	};
}

void Replication::Write(const leveldb::WriteBatch* batch, bool repl){
	Handler handler;
	handler.noop  = !repl;
	handler.repl_ = this;
	handler.sequence_ = WriteBatchInternal::Sequence(batch);
	batch->Iterate(&handler);
}

}
