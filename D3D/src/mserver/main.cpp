#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>

int main(int argc, char **argv) {
	int sockfd;
	struct sockaddr_in servaddr;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("could not make socket fd\n");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(32000);

	bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	listen(sockfd, 1024);

	while(true) {
		struct sockaddr_in client_addr;
		socklen_t client_addr_length;
		int client = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_length);;
		printf("new client connection\n");
	}

}
