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
		uint64_t copy_count;
		uint64_t sync_count;
		
		std::string id_;

		SSDB *ssdb;
		Link *link;
		leveldb::DB* meta_db;
		std::string master_ip;
		int master_port;
		bool is_mirror;
		char log_type;

		std::string status_key();
		void load_status();
		void save_status();

		volatile bool thread_quit;
		pthread_t run_thread_tid;
		static void* _run_thread(void *arg);
		
		int proc(const std::vector<Bytes> &req);
		int proc_noop(const Binlog &log, const std::vector<Bytes> &req);
		int proc_copy(const Binlog &log, const std::vector<Bytes> &req);
		int proc_sync(const Binlog &log, const std::vector<Bytes> &req);

		unsigned int connect_retry;
		int connect();
		bool connected(){
			return link != NULL;
		}
	public:
		Slave(SSDB *ssdb, leveldb::DB* meta_db, const char *ip, int port, bool is_mirror=false);
		~Slave();
		void start();
		void stop();
		
		void set_id(const std::string &id);
		std::string stats() const;
};

#endif
