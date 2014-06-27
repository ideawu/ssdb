#ifndef SSDB_ITERATOR_H_
#define SSDB_ITERATOR_H_

#include <inttypes.h>
#include <string>
#include "leveldb/iterator.h"
#include "leveldb/slice.h"
#include "util/bytes.h"

class Iterator{
	public:
		enum Direction{
			FORWARD, BACKWARD
		};
	private:
		leveldb::Iterator *it;
		std::string end;
		uint64_t limit;
		bool is_first;
		int direction;
	public:
		Iterator(leveldb::Iterator *it,
				const std::string &end,
				uint64_t limit,
				Direction direction=Iterator::FORWARD);
		~Iterator();
		bool skip(uint64_t offset);
		bool next();

		Bytes key(){
			return it->key();
		}

		Bytes val(){
			return it->value();
		}
};

#endif
