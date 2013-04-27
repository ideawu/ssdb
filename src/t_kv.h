#ifndef SSDB_KV_H_
#define SSDB_KV_H_

#include "ssdb.h"

static inline
std::string encode_kv_key(const Bytes &key){
	std::string buf;
	buf.append(1, DataType::KV);
	buf.append(key.data(), key.size());
	return buf;
}

static inline
int decode_kv_key(const Bytes &slice, std::string *key){
	Decoder decoder(slice.data(), slice.size());
	if(decoder.skip(1) == -1){
		return -1;
	}
	if(decoder.read_data(key) == -1){
		return -1;
	}
	return 0;
}


class KIterator{
	private:
		Iterator *it;
		bool return_val_;
	public:
		std::string key;
		std::string val;
		// Bytes raw_key;
		// Bytes raw_val;

		KIterator(Iterator *it){
			this->it = it;
			this->return_val_ = true;
		}

		~KIterator(){
			delete it;
		}

		void return_val(bool onoff){
			this->return_val_ = onoff;
		}

		bool next(){
			while(it->next()){
				Bytes ks = it->key();
				Bytes vs = it->val();
				//dump(ks.data(), ks.size(), "z.next");
				//dump(vs.data(), vs.size(), "z.next");
				if(ks.data()[0] != DataType::KV){
					return false;
				}
				if(decode_kv_key(ks, &this->key) == -1){
					continue;
				}
				if(return_val_){
					this->val.assign(vs.data(), vs.size());
				}
				return true;
			}
			return  false;
		}
};


#endif
