#ifndef _SOCKUTIL_H_
#define _SOCKUTIL_H_

#include <winsock.h>

int recvall(SOCKET socket, char *buffer, size_t size, int args);
int sendall(SOCKET socket, char *buffer, size_t size, int args);

#endif