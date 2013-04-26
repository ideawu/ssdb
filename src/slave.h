#ifndef SSDB_SLAVE_H_
#define SSDB_SLAVE_H_

#include <stdint.h>
#include <string>
#include <pthread.h>
#include <vector>
#include "ssdb.h"
#include "link.h"

class Slave{
	private:
		uint64_t last_seq;
		std::string last_key;

		const SSDB *ssdb;
		Link *link;
		leveldb::DB* meta_db;
		std::string master_ip;
		int master_port;
		bool is_mirror;

		std::string status_key();
		void load_status();
		void save_status();

		volatile bool thread_quit;
		pthread_t run_thread_tid;
		static void* _run_thread(void *arg);
		
		int proc(const std::vector<Bytes> &req);
		int proc_noop(const Binlog &log, const std::vector<Bytes> &req);
		int proc_dump(const Binlog &log, const std::vector<Bytes> &req);
		int proc_sync(const Binlog &log, const std::vector<Bytes> &req);

		int connect_retry;
		int connect();
		bool connected(){
			return link != NULL;
		}
	public:
		Slave(const SSDB *ssdb, leveldb::DB* meta_db, const char *ip, int port, bool is_mirror=false);
		~Slave();
		void start();
		void stop();
};

#endif
