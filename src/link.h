#ifndef SSDB_LINK_H_
#define SSDB_LINK_H_

#include <vector>
#include <unistd.h>

#include "util/bytes.h"

class Link{
	private:
		int sock;
		bool noblock_;
		std::vector<Bytes> recv_data;

	public:
		static int min_recv_buf;
		static int min_send_buf;

		Buffer *input;
		Buffer *output;

		Link(bool is_server=false);
		~Link();
		void close();
		void nodelay(bool enable=true);
		// noblock(true) is supposed to corperate with IO Multiplex,
		// otherwise, flush() may cause a lot unneccessary write calls.
		void noblock(bool enable=true);

		int fd() const{
			return sock;
		}

		static Link* connect(const char *ip, int port);
		static Link* listen(const char *ip, int port);
		Link* accept();

		// read network data info buffer
		int read();
		int write();
		// flush buffered data to network
		int flush();

		/**
		 * parse received data, and return -
		 * NULL: error
		 * empty vector: recv not ready
		 * vector<Bytes>: recv ready
		 */
		const std::vector<Bytes>* recv();

		// need to call flush to ensure all data has flush into network
		int send(const std::vector<std::string> &packet);
		int send(const std::vector<Bytes> &packet);
		int send(const Bytes &s1);
		int send(const Bytes &s1, const Bytes &s2);
		int send(const Bytes &s1, const Bytes &s2, const Bytes &s3);
		int send(const Bytes &s1, const Bytes &s2, const Bytes &s3, const Bytes &s4);
		int send(const Bytes &s1, const Bytes &s2, const Bytes &s3, const Bytes &s4, const Bytes &s5);

		const std::vector<Bytes>* last_recv(){
			return &recv_data;
		}
};

#endif
