/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef SSDB_QUEUE_H_
#define SSDB_QUEUE_H_

#include "ssdb_impl.h"

const uint64_t QFRONT_SEQ = 2;
const uint64_t QBACK_SEQ  = 3;
const uint64_t QITEM_MIN_SEQ = 10000;
const uint64_t QITEM_MAX_SEQ = 9223372036854775807ULL;
const uint64_t QITEM_SEQ_INIT = QITEM_MAX_SEQ/2;

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
