/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "proc.h"
#include "server.h"
#include "../util/log.h"

ProcMap::ProcMap(){
}

ProcMap::~ProcMap(){
	proc_map_t::iterator it;
	for(it=proc_map.begin(); it!=proc_map.end(); it++){
		delete it->second;
	}
	proc_map.clear();
}

void ProcMap::set_proc(const std::string &c, proc_t proc){
	this->set_proc(c, "t", proc);
}

void ProcMap::set_proc(const std::string &c, const char *sflags, proc_t proc){
	Command *cmd = this->get_proc(c);
	if(!cmd){
		cmd = new Command();
		cmd->name = c;
		proc_map[cmd->name] = cmd;
	}
	cmd->proc = proc;
	cmd->flags = 0;
	for(const char *p=sflags; *p!='\0'; p++){
		switch(*p){
			case 'r':
				cmd->flags |= Command::FLAG_READ;
				break;
			case 'w': // w 必须和 t 同时出现, 因为某些写操作依赖单线程
				cmd->flags |= Command::FLAG_WRITE;
				cmd->flags |= Command::FLAG_THREAD;
				break;
			case 'b':
				cmd->flags |= Command::FLAG_BACKEND;
				break;
			case 't':
				cmd->flags |= Command::FLAG_THREAD;
				break;
		}
	}
}

Command* ProcMap::get_proc(const Bytes &str){
	proc_map_t::iterator it = proc_map.find(str);
	if(it != proc_map.end()){
		return it->second;
	}
	return NULL;
}
