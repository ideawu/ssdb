/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef SSDB_NSET_H_
#define SSDB_NSET_H_

#include "ssdb_impl.h"

#define encode_score(s) big_endian((uint64_t)(s))
#define decode_score(s) big_endian((uint64_t)(s))

static inline
std::string encode_nsize_key(const Bytes &name){
	std::string buf;
	buf.append(1, DataType::NSIZE);
	buf.append(name.data(), name.size());
	return buf;
}

inline static
int decode_nsize_key(const Bytes &slice, std::string *name){
	Decoder decoder(slice.data(), slice.size());
	if(decoder.skip(1) == -1){
		return -1;
	}
	if(decoder.read_data(name) == -1){
		return -1;
	}
	return 0;
}


// type, len, key, score
static inline
std::string encode_nscore_key(const Bytes &name, const Bytes &score){
	std::string buf;
	buf.append(1, DataType::NSET);
	buf.append(1, (uint8_t)name.size());
	buf.append(name.data(), name.size());

	int64_t s = score.Int64();
	if(s < 0){
		buf.append(1, '-');
	}else{
		buf.append(1, '=');
	}
	s = encode_score(s);

	buf.append((char *)&s, sizeof(int64_t));

	return buf;
}

static inline
int decode_nscore_key(const Bytes &slice, std::string *name, std::string *score){
	Decoder decoder(slice.data(), slice.size());
	if(decoder.skip(1) == -1){
		return -1;
	}
	if(decoder.read_8_data(name) == -1){
		return -1;
	}
	if(decoder.skip(1) == -1){
		return -1;
	}
	int64_t s;
	if(decoder.read_int64(&s) == -1){
		return -1;
	}else{
		if(score != NULL){
			s = decode_score(s);
			score->assign(str(s));
		}
	}

	return 0;
}

#endif
