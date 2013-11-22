#ifndef SSDB_ZSET_H_
#define SSDB_ZSET_H_

#include "include.h"
#include "ssdb.h"
#include "util/strings.h"

#define encode_score(s) big_endian((uint64_t)(s))
#define decode_score(s) big_endian((uint64_t)(s))

static inline
std::string encode_zsize_key(const Bytes &name){
	std::string buf;
	buf.append(1, DataType::ZSIZE);
	buf.append(name.data(), name.size());
	return buf;
}

inline static
int decode_zsize_key(const Bytes &slice, std::string *name){
	Decoder decoder(slice.data(), slice.size());
	if(decoder.skip(1) == -1){
		return -1;
	}
	if(decoder.read_data(name) == -1){
		return -1;
	}
	return 0;
}

static inline
std::string encode_zset_key(const Bytes &name, const Bytes &key){
	std::string buf;
	buf.append(1, DataType::ZSET);
	buf.append(1, (uint8_t)name.size());
	buf.append(name.data(), name.size());
	buf.append(1, (uint8_t)key.size());
	buf.append(key.data(), key.size());
	return buf;
}

static inline
int decode_zset_key(const Bytes &slice, std::string *name, std::string *key){
	Decoder decoder(slice.data(), slice.size());
	if(decoder.skip(1) == -1){
		return -1;
	}
	if(decoder.read_8_data(name) == -1){
		return -1;
	}
	if(decoder.read_8_data(key) == -1){
		return -1;
	}
	return 0;
}

// type, len, key, score, =, val
static inline
std::string encode_zscore_key(const Bytes &key, const Bytes &val, const Bytes &score){
	std::string buf;
	buf.append(1, DataType::ZSCORE);
	buf.append(1, (uint8_t)key.size());
	buf.append(key.data(), key.size());

	int64_t s = score.Int64();
	if(s < 0){
		buf.append(1, '-');
	}else{
		buf.append(1, '=');
	}
	s = encode_score(s);

	buf.append((char *)&s, sizeof(int64_t));
	buf.append(1, '=');
	buf.append(val.data(), val.size());
	return buf;
}

static inline
int decode_zscore_key(const Bytes &slice, std::string *name, std::string *key, std::string *score){
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
			char buf[21] = {0};
			snprintf(buf, sizeof(buf), "%" PRId64 "", s);
			score->assign(buf);
		}
	}
	if(decoder.skip(1) == -1){
		return -1;
	}
	if(decoder.read_data(key) == -1){
		return -1;
	}
	return 0;
}


class ZIterator{
	private:
		Iterator *it;
	public:
		std::string name;
		std::string key;
		std::string score;

		ZIterator(Iterator *it, const Bytes &name){
			this->it = it;
			this->name.assign(name.data(), name.size());
		}

		~ZIterator(){
			delete it;
		}
		
		bool skip(uint64_t offset){
			while(offset-- > 0){
				if(this->next() == false){
					return false;
				}
			}
			return true;
		}

		bool next(){
			while(it->next()){
				Bytes ks = it->key();
				//Bytes vs = it->val();
				//dump(ks.data(), ks.size(), "z.next");
				//dump(vs.data(), vs.size(), "z.next");
				if(ks.data()[0] != DataType::ZSCORE){
					return false;
				}
				if(decode_zscore_key(ks, NULL, &key, &score) == -1){
					continue;
				}
				return true;
			}
			return false;
		}
};


#endif
