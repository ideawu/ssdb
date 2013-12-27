#ifndef SSDB_QUEUE_H_
#define SSDB_QUEUE_H_

#include "ssdb.h"

static uint64_t QSIZE_SEQ  = 1;
static uint64_t QFRONT_SEQ = 2;
static uint64_t QBACK_SEQ  = 3;
static uint64_t QITEM_FIRST_SEQ = ((4294967295/2) * 4294967295);

inline static std::string qitem_key(const Bytes &name, uint64_t seq){
	assert(seq >= QFIRST_SEQ);
	std::string buf;
	buf.append(1, DataType::QUEUE);
	buf.append(1, (uint8_t)name.size());
	buf.append(name.data(), name.size());
	seq = big_endian(seq);
	buf.append((char *)&seq, sizeof(uint64_t));
	return buf;
}

#endif
