#ifdef _SOCKUTIL_H_
#define _SOCKUTIL_H_

int recvall(int socket, void *buffer, size_t size, int args);
int sendall(int socket, void *buffer, size_t size, int args);

#endif
