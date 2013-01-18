#ifndef SSDB_ITERATOR_H_
#define SSDB_ITERATOR_H_

class Iterator{
	public:
		enum{
			FORWARD, BACKWARD
		};
		//const static int FORWARD = 1;
		//const static int BACKWARD = 2;
	private:
		leveldb::Iterator *it;
		std::string end;
		int limit;
		bool begin;
		int direction;
	public:
		Iterator(leveldb::Iterator *it,
				const std::string &end,
				int limit,
				int direction=Iterator::FORWARD){
			this->it = it;
			this->end = end;
			this->limit = limit;
			this->begin = true;
			this->direction = direction;
		}

		~Iterator(){
			delete it;
		}

		bool next(){
			if(begin){
				begin = false;
			}else{
				if(direction == FORWARD){
					it->Next();
				}else{
					it->Prev();
				}
			}

			if(limit <= 0 || !it->Valid()){
				return false;
			}
			if(direction == FORWARD){
				if(!end.empty() && it->key() >= end){
					return false;
				}
			}else{
				if(!end.empty() && it->key() <= end){
					return false;
				}
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
