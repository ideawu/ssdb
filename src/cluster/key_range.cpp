#include "key_range.h"
#include "../util/strings.h"

KeyRange::KeyRange(){
}
	
KeyRange::KeyRange(const std::string &start, const std::string &end){
	this->start = start;
	this->end = end;
}
	
bool KeyRange::check_overlapped(const KeyRange &range){
	if(!this->start.empty() && !range.end.empty() && this->start >= range.end){
		return false;
	}
	if(!this->end.empty() && !range.start.empty() && this->end <= range.start){
		return false;
	}
	return true;
}

std::string KeyRange::str(){
	return "(\"" + str_escape(start) + "\" - \"" + str_escape(end) + "\"]";
}
