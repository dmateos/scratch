#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <string>

#include <stdio.h>
#include <strings.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/errno.h>

class Connection {
	public:
		Connection(std::string server);
		void get_message();
	private:
		int sock_fd;
};

#endif
