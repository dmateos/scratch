#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "networking.h"

int socket_listen() {
    struct sockaddr_in pin;
    int sd;

    memset(&pin, '\0', sizeof(pin));
    pin.sin_family = AF_INET;
    pin.sin_addr.s_addr = INADDR_ANY;
    pin.sin_port = htons(13373);

    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "socket error\n");
        exit(1);
    }
    if(bind(sd, (struct sockaddr*)&pin, sizeof pin) == -1) {
        fprintf(stderr, "bind error\n");
        exit(1);
    }
    if(listen(sd, 1024) == -1) {
        fprintf(stderr, "listen error\n");
        exit(1);
    }
    return sd;
}

int socket_accept(int sd) {
    struct sockaddr_in cdetails;
    int cd, csize;

    memset(&cdetails, '\0', sizeof cdetails);
    csize = 0;

    if((cd = accept(sd, (struct sockaddr*)&cdetails, &csize)) == -1) {
        exit(1);
    }

    return cd;
}

int socket_connect(const char *hostname) {
    struct sockaddr_in pin;
    struct hostent *hp;
    int sd;

    if((hp = gethostbyname(hostname)) == 0) {
        exit(1);
    }

    memset(&pin, '\0', sizeof(pin));
    pin.sin_family = AF_INET;
    pin.sin_addr.s_addr = ((struct in_addr*)(hp->h_addr))->s_addr;
    pin.sin_port = htons(13373);

    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        exit(1);
    }
    if(connect(sd, (struct sockaddr*)&pin, sizeof(pin)) == -1) {
        exit(1);
    }
    return sd;
}
