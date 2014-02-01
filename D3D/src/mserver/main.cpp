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

enum commands {
	HELLO = 0x01,
	GOODBYE,

	NEW_PPOS,
	UPD_PPOS,

	NEW_OPOS,
	UPD_OPOS,
};

struct packet {
	char cmd;
	int length;
	void *data;
};

struct thread_args {
	int client_sd;
	sockaddr_in client_addr;
};

void *handle_client(void *targs) {
	thread_args *args = (thread_args*)targs;
	printf("new thread %d\n", args->client_sd);
	while(true) {
		packet hello_packet;
		hello_packet.cmd = HELLO;
		hello_packet.length = 0;
		hello_packet.data = NULL;

		if(send(args->client_sd, (void*)&hello_packet, sizeof(hello_packet), 0) == -1) {
			printf("couldnt hello msg to client\n");
		}
		sleep(10);
	}
	pthread_exit(NULL);
}

int main(int argc, char **argv) {
	int sockfd;
	struct sockaddr_in servaddr;
	pthread_t threads[32];
	thread_args args[32];

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

	for(long i = 0; i < 32; i++) {
		struct sockaddr_in client_addr;
		socklen_t client_addr_length;
		int client = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_length);;
		printf("new client connection\n");

		args[i].client_sd = client;
		args[i].client_addr = client_addr;
		int rc = pthread_create(&threads[i], NULL, handle_client, (void*)&args[i]);
		if(rc) {
			printf("could not make thread\n");
			exit(1);
		}
	}
	pthread_exit(NULL);
}
