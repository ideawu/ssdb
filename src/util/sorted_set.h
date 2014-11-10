/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef UTIL_SORTED_SET_H
#define UTIL_SORTED_SET_H

#include <inttypes.h>
#include <string>
#include <map>
#include <set>

class SortedSet
{
public:
	bool empty() const{
		return size() == 0;
	}
	int size() const;
	int add(const std::string &key, int64_t score);
	// 0: not found, 1: found and deleted
	int del(const std::string &key);
	// the first item is copied into key if SortedSet not empty
	int front(std::string *key, int64_t *score=NULL) const;
	int back(std::string *key, int64_t *score=NULL) const;
	int64_t max_score() const;
	int pop_front();
	int pop_back();
	
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
