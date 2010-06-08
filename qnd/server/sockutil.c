#include <sys/socket.h>
#include "sockutil.h"

int recvall(int socket, void *buffer, size_t size, int args) {
    unsigned int total, n, left; 
    
    total = n = 0;
    left = size;

    while(total < size) {
        n = recv(socket, buffer+total, left, args);
        if(n == 0 || n == -1)
            return -1;
        total += n;
        left -= n;
    }
    return total;
}

int sendall(int socket, void *buffer, size_t size, int args) {
    unsigned int total, n, left;
    
    total = n = 0;
    left = size;

    while(total < size) {
        n = send(socket, buffer+total, left, args);
        if(n == 0 || n == -1)
            return -1;
        total += n;
        left -= n;
    }
    return total;
}
