#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include <vector>
#include "network_protocol.h"

void *handle_client(void *targs) {
	thread_args *args = (thread_args*)targs;
	printf("client connection %d\n", args->client_sd);

	packet hello_packet;
	hello_packet.cmd = HELLO;
	hello_packet.length = 0;

	if(send(args->client_sd, (void*)&hello_packet, sizeof(hello_packet), 0) == -1) {
		printf("couldnt hello msg to client\n");
	}

	packet in_packet;
	while(true) {
		recv(args->client_sd, &in_packet, sizeof(in_packet), 0); //TODO check for discon
		switch(in_packet.cmd) {
			case HELLO:
				printf("new hello packet with %d size\n", in_packet.length);
				break;
			default:
				printf("unknown packet with %d size\n", in_packet.length);
				break;
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char **argv) {
	int sockfd;
	struct sockaddr_in servaddr;
	std::vector<pthread_t> threads;
	std::vector<thread_args*> args;

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

		thread_args *arg = new thread_args();
		arg->client_sd = client;
		arg->client_addr = client_addr;

		pthread_t thread;
		int rc = pthread_create(&thread, NULL, handle_client, (void*)arg);
		threads.push_back(thread);
		args.push_back(arg);
		if(rc) {
			printf("could not make thread\n");
			exit(1);
		}
	}
	pthread_exit(NULL);
}
