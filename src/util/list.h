/*
Copyright (c) 2012-2014 The icomet Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef UTIL_LIST_H
#define UTIL_LIST_H

template <class T>
class LinkedList{
public:
	class Iterator{
	private:
		T p;
	public:
		friend class LinkedList;
		
		T next(){
			T ret = p;
			if(p){
				p = p->next;
			}
			return ret;
		}
	};
	friend class Iterator;
public:
	int size;
	T head;
	T tail;
	
	LinkedList(){
		size = 0;
		head = NULL;
		tail = NULL;
	}
	
	Iterator iterator(){
		Iterator it;
		it.p = this->head;
		return it;
	}
	
	bool empty() const{
		return size == 0;
	}
	
	void remove(T t){
		this->size --;
		if(t->prev){
			t->prev->next = t->next;
		}
		if(t->next){
			t->next->prev = t->prev;
		}
		if(this->head == t){
			this->head = t->next;
		}
		if(this->tail == t){
			this->tail = t->prev;
		}
	}
	
	T pop_front(){
		T t = this->head;
		this->remove(t);
		return t;
	}

	void push_back(T t){
		this->size ++;
		t->prev = this->tail;
		t->next = NULL;
		if(this->tail){
			this->tail->next = t;
		}else{ // both head and tail is empty
			this->head = t;
		}
		this->tail = t;
	}
};


#endif
