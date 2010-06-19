#ifndef _NETWORKING_H_
#define _NETWORKING_H_

int socket_listen();
int socket_accept(int sd);
int socket_connect(const char *hostname);

#endif
