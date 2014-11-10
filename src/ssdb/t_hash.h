/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef SSDB_HASH_H_
#define SSDB_HASH_H_

#include "ssdb_impl.h"

inline static
std::string encode_hsize_key(const Bytes &name){
	std::string buf;
	buf.append(1, DataType::HSIZE);
	buf.append(name.data(), name.size());
	return buf;
}

inline static
int decode_hsize_key(const Bytes &slice, std::string *name){
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
std::string encode_hash_key(const Bytes &name, const Bytes &key){
	std::string buf;
	buf.append(1, DataType::HASH);
	buf.append(1, (uint8_t)name.size());
	buf.append(name.data(), name.size());
	buf.append(1, '=');
	buf.append(key.data(), key.size());
	return buf;
}

inline static
int decode_hash_key(const Bytes &slice, std::string *name, std::string *key){
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
	if(decoder.read_data(key) == -1){
		return -1;
	}
	return 0;
}

#endif
