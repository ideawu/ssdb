#ifndef SSDB_ZSET_H_
#define SSDB_ZSET_H_

#include <stdint.h>
#include <stdlib.h>
#include "ssdb.h"
#include "util/strings.h"

#define encode_score(s) big_endian((uint64_t)(s))
#define decode_score(s) big_endian((uint64_t)(s))

static inline
std::string encode_zsize_key(const Bytes &name){
	std::string buf;
	buf.append(1, DataType::ZSCORE);
	buf.append(name.data(), name.size());
	return buf;
}

static inline
std::string encode_zs_key(const Bytes &key, const Bytes &val){
	std::string buf;
	buf.append(1, DataType::ZSCORE);
	buf.append(1, (uint8_t)key.size());
	buf.append(key.data(), key.size());
	buf.append(1, (uint8_t)val.size());
	buf.append(val.data(), val.size());
	return buf;
}

static inline
std::string encode_z_key(const Bytes &key, const Bytes &val, const Bytes &score){
	std::string buf;
	buf.append(1, DataType::ZSET);
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
int decode_z_key(const Bytes &slice, std::string *key, std::string *val, std::string *score){
	int size = slice.size();
	if(size < 1){
		return -1;
	}
	const char *p = slice.data();
	p += 1;
	size -= 1;

	int len;
	len = (uint8_t)p[0];
	p += 1;
	size -= 1;
	if(size < len){
		return -1;
	}
	if(key){
		key->assign(p, len);
	}
	p += len;
	size -= len;

	if(size < SSDB_SCORE_WIDTH){
		return -1;
	}
	if(score){
		int64_t s = *(int64_t *)(p + 1);
		s = decode_score(s);
		char buf[21] = {0};
		snprintf(buf, sizeof(buf), "%lld", (long long)s);
		score->assign(buf);
	}
	p += SSDB_SCORE_WIDTH;
	size -= SSDB_SCORE_WIDTH;

	if(size < 1){
		return -1;
	}
	p += 1;
	size -= 1;

	if(val){
		val->assign(p, size);
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

		bool next(){
			while(it->next()){
				Bytes ks = it->key();
				Bytes vs = it->val();
				//dump(ks.data(), ks.size(), "z.next");
				//dump(vs.data(), vs.size(), "z.next");
				if(ks.data()[0] != DataType::ZSET){
					return false;
				}
				if(decode_z_key(ks, NULL, &key, &score) == -1){
					continue;
				}
				return true;
			}
			return false;
		}
};


#endif
