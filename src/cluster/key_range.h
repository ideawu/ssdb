#ifndef SSDB_CLUSTER_KEY_RANGE_H
#define SSDB_CLUSTER_KEY_RANGE_H

/**
 * (start, end], start is exclusive, end is inclusive
 */
class KeyRange{
public:
	std::string start;
	std::string end;

	KeyRange(){
	}
	
	KeyRange(const std::string &start, const std::string &end){
		this->start = start;
		this->end = end;
	}
	
	bool check_overlapped(const KeyRange &range){
		if(!this->start.empty() && !range.end.empty() && this->start >= range.end){
			return false;
		}
		if(!this->end.empty() && !range.start.empty() && this->end <= range.start){
			return false;
		}
		return true;
	}
private:
};

#endif
