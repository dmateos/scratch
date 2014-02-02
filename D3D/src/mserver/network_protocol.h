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
	int oid;
	float x, y, z;
};

#endif
