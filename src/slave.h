#ifndef SSDB_SLAVE_H_
#define SSDB_SLAVE_H_

#include <stdint.h>
#include <string>
#include "ssdb.h"
#include "link.h"

class Slave{
	private:
		uint64_t next_seq;
		std::string last_key;

		const SSDB *ssdb;
		leveldb::DB* meta_db;
		std::string master_ip;
		int master_port;

		static void* _run_thread(void *arg);
		std::string status_key();
		void load_status();
		void save_status();
	public:
		Slave(const SSDB *ssdb, leveldb::DB* meta_db, const char *ip, int port);
		~Slave();
		void start();
};

#endif
