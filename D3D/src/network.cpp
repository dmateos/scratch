#include "network.h"

//
// Packet
//
Packet::Packet() {

}

//
// Connection
//
Connection::Connection(std::string server) {
	int sockfd;
	struct sockaddr_in servaddr;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("cant make socket connection");
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(server.c_str());
	servaddr.sin_port = htons(32000);

	if(connect(sockfd, (const sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
		printf("could not connect to remote server %s\n", server.c_str());
	}
}
