#ifndef SSDB_HASH_H_
#define SSDB_HASH_H_

#include "ssdb.h"

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



class HIterator{
	private:
		Iterator *it;
		bool return_val_;
	public:
		std::string name;
		std::string key;
		std::string val;

		HIterator(Iterator *it, const Bytes &name){
			this->it = it;
			this->name.assign(name.data(), name.size());
			this->return_val_ = true;
		}

		~HIterator(){
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
				if(ks.data()[0] != DataType::HASH){
					return false;
				}
				std::string n;
				if(decode_hash_key(ks, &n, &key) == -1){
					continue;
				}
				if(n != this->name){
					return false;
				}
				if(return_val_){
					this->val.assign(vs.data(), vs.size());
				}
				return true;
			}
			return false;
		}

};

#endif
