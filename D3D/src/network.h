#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <string>

#include <stdio.h>
#include <strings.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Packet {
	public:
	Packet();
};

class Connection {
	public:
	Connection(std::string server);
};

#endif
