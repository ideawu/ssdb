#ifndef SSDB_ITERATOR_H_
#define SSDB_ITERATOR_H_

class Iterator{
	private:
		leveldb::Iterator *it;
		std::string end;
		int limit;
		bool begin;
	public:
		Iterator(leveldb::Iterator *it, const std::string &end, int limit){
			this->it = it;
			this->end = end;
			this->limit = limit;
			this->begin = true;
		}

		~Iterator(){
			delete it;
		}

		bool next(){
			if(begin){
				begin = false;
			}else{
				it->Next();
			}

			if(limit <= 0 || !it->Valid()){
				return false;
			}
			if(!end.empty() && it->key() >= end){
				return false;
			}
			limit --;
			return true;
		}

		Bytes key(){
			return it->key();
		}

		Bytes val(){
			return it->value();
		}
};

#endif
