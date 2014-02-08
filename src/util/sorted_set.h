#ifndef UTIL_SORTED_SET_H
#define UTIL_SORTED_SET_H

#include <inttypes.h>
#include <string>
#include <map>
#include <set>

class SortedSet
{
public:
	int size() const;
	int add(const std::string &key, int64_t score);
	int del(const std::string &key);
	int front(const std::string **key, int64_t *score=NULL) const;
	int pop_front();
	
	/*
	class Iterator
	{
	public:
		bool next();
		const std::string& key();
		int64_t score();
	};
	
	Iterator begin();
	*/

private:
	struct Item
	{
		std::string key;
		int64_t score;
		
		bool operator<(const Item& b) const{
			return this->score < b.score
				|| (this->score == b.score && this->key < b.key);
		}
	};
	
	std::map<std::string, std::set<Item>::iterator> existed;
	std::set<Item> sorted_set;
};


/*
TODO: HashedWheel
Each item is linked in two list, one is slot list, the other
one is total list.
*/
/*
template <class T>
class SortedList
{
public:
	void add(const T data, int64_t score);
	T front();
	void pop_front();

	class Item
	{
	public:
		int64_t score;
		Item *prev;
		Item *next;
		//Item *slot_prev;
		//Item *slot_next;
		T data;
	};
};
*/

#endif
