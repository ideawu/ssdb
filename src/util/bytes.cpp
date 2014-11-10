/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "bytes.h"

Buffer::Buffer(int total){
	size_ = 0;
	total_ = origin_total = total;
	buf = (char *)malloc(total);
	data_ = buf;
}

Buffer::~Buffer(){
	free(buf);
}

void Buffer::nice(){
	// 保证不改变后半段的数据, 以便使已生成的 Bytes 不失效.
	if(data_ - buf > total_/2){
		memcpy(buf, data_, size_);
		data_ = buf;
	}
}

int Buffer::grow(){ // 扩大缓冲区
	int n;
	if(total_ < 8 * 1024){
		n = 8 * 1024;
	}else if(total_ < 512 * 1024){
		n = 8 * total_;
	}else{
		n = 2 * total_;
	}
	//log_debug("Buffer resize %d => %d", total_, n);
	char *p = (char *)realloc(buf, n);
	if(p == NULL){
		return -1;
	}
	data_ = p + (data_ - buf);
	buf = p;
	total_ = n;
	return total_;
}

std::string Buffer::stats() const{
	char str[1024 * 32];
	str[0] = '\n';
	sprintf(str, "total: %d, data: %d, size: %d, slot: %d",
		total_, (int)(data_ - buf), size_, (int)(slot() - buf));
	return std::string(str);
}

int Buffer::read_record(Bytes *s){
	char *head = this->data();
	char *body = (char *)memchr(head, '\n', this->size_);
	if(body == NULL){
		return 0;
	}
	body ++;

	int head_len = body - head;
	if(head[0] < '0' || head[0] > '9'){
		return -1;
	}

	char head_str[20];
	if(head_len + 1 > (int)sizeof(head_str)){
		return -1;
	}
	memcpy(head_str, head, head_len - 1); // no '\n'
	head_str[head_len - 1] = '\0';

	int body_len = atoi(head_str);
	if(body_len < 0){
		return -1;
	}

	char *p = body + body_len;
	if(this->size_ >= head_len + body_len + 1){
		if(p[0] == '\n'){
			this->size_ -= head_len + body_len + 1;
			*s = Bytes(body, body_len);
			return 1;
		}else if(p[0] == '\r'){
			if(this->size_ >= head_len + body_len + 2){
				if(p[1] == '\n'){
					this->size_ -= head_len + body_len + 2;
					*s = Bytes(body, body_len);
					return 1;
				}else{
					return -1;
				}
			}
		}else{
			return -1;
		}
	}
	return 0;
}

int Buffer::append_record(const Bytes &s){
	// 16 is the maximum length of literal string of s.size()
	int size = 16 + s.size() + 1;
	while(size > this->space()){
		if(this->grow() == -1){
			return -1;
		}
	}

	char len[16];
	int num = snprintf(len, sizeof(len), "%d\n", (int)s.size());

	char *p = this->slot();
	memcpy(p, len, num);
	p += num;

	memcpy(p, s.data(), s.size());
	p += s.size();

	*p = '\n';
	p += 1;
	this->size_ += (num + s.size() + 1);
	return (num + s.size() + 1);
}

int Buffer::append(char c){
	while(1 > this->space()){
		if(this->grow() == -1){
			return -1;
		}
	}

	*(this->slot()) = c;
	size_ += 1;
	return 1;
}

int Buffer::append(const void *p, int size){
	while(size > this->space()){
		if(this->grow() == -1){
			return -1;
		}
	}

	memcpy(this->slot(), p, size);
	size_ += size;
	return size;
}

int Buffer::append(const char *p){
	return this->append(p, strlen(p));
}

int Buffer::append(const Bytes &s){
	return this->append(s.data(), s.size());
}
