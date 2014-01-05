#ifndef SSDB_QUEUE_H_
#define SSDB_QUEUE_H_

#include "ssdb.h"

//static uint64_t QSIZE_SEQ  = 1;
static uint64_t QFRONT_SEQ = 2;
static uint64_t QBACK_SEQ  = 3;
static uint64_t QITEM_MIN_SEQ = 10000;
static uint64_t QITEM_MAX_SEQ = 9223372036854775807ULL;

inline static
std::string encode_qsize_key(const Bytes &name){
	std::string buf;
	buf.append(1, DataType::QSIZE);
	buf.append(name.data(), name.size());
	return buf;
}

inline static
int decode_qsize_key(const Bytes &slice, std::string *name){
	Decoder decoder(slice.data(), slice.size());
	if(decoder.skip(1) == -1){
		return -1;
	}
	if(decoder.read_data(name) == -1){
		return -1;
	}
	return 0;
}

inline static
std::string encode_qitem_key(const Bytes &name, uint64_t seq){
	std::string buf;
	buf.append(1, DataType::QUEUE);
	buf.append(1, (uint8_t)name.size());
	buf.append(name.data(), name.size());
	seq = big_endian(seq);
	buf.append((char *)&seq, sizeof(uint64_t));
	return buf;
}

inline static
int decode_qitem_key(const Bytes &slice, std::string *name, uint64_t *seq){
	Decoder decoder(slice.data(), slice.size());
	if(decoder.skip(1) == -1){
		return -1;
	}
	if(decoder.read_8_data(name) == -1){
		return -1;
	}
	if(decoder.read_uint64(seq) == -1){
		return -1;
	}
	*seq = big_endian(*seq);
	return 0;
}

#endif
