#ifndef SSDB_PROC_H_
#define SSDB_PROC_H_

class Bytes;
struct Command;

class ProcMap
{
public:
	ProcMap();
	~ProcMap();
	Command* find(const Bytes &str);
};

#endif
