#ifndef SSDB_CLUSTER_KEY_RANGE_H
#define SSDB_CLUSTER_KEY_RANGE_H

#include <string>

/**
 * (start, end], start is exclusive, end is inclusive
 */
class KeyRange{
public:
	std::string start;
	std::string end;

	KeyRange();
	KeyRange(const std::string &start, const std::string &end);
	bool check_overlapped(const KeyRange &range);
	
	std::string str();
private:
};

#endif
