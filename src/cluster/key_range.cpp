#include "key_range.h"
#include "../util/strings.h"

KeyRange::KeyRange(){
}
	
KeyRange::KeyRange(const std::string &start, const std::string &end){
	this->start = start;
	this->end = end;
}

std::string KeyRange::str() const{
	return "(\"" + str_escape(start) + "\" - \"" + str_escape(end) + "\"]";
}

bool KeyRange::check_overlapped(const KeyRange &range) const{
	if(!this->start.empty() && !range.end.empty() && this->start >= range.end){
		return false;
	}
	if(!this->end.empty() && !range.start.empty() && this->end <= range.start){
		return false;
	}
	return true;
}

bool KeyRange::empty() const{
	return start == "" && end == "";
}
