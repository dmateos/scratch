#ifndef _NETWORK_PROTOCOL_H_
#define _NETWORK_PROTOCOL_H_

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

#endif
