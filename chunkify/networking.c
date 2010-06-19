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
        return -1;
    }
    if(bind(sd, (struct sockaddr*)&pin, sizeof pin) == -1) {
        return -1;
    }
    if(listen(sd, 1024) == -1) {
        return -1;
    }
    return sd;
}

int socket_accept(int sd) {
    struct sockaddr_in cdetails;
    int cd, csize;

    if((cd = accept(sd, (struct sockaddr*)&cdetails, &csize)) == -1) {
        return -1;
    }

    return cd;
}
