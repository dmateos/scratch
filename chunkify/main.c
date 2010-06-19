#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "chunkify.h"
#include "networking.h"

void server(const char *filepath) {
    chunk_t *data;
    int serversock, clientsock, chunkc;
    char hellomsg[1024];

    /* Chunkify the data and prep a hello message. */
    printf("%s\n", filepath);
    data = chunkify(filepath, &chunkc);
    snprintf(hellomsg, 1024, "hello:%d %d\n", chunkc, CHUNKSIZE);

    /* Accept a client. */
    serversock = socket_listen();
    clientsock = socket_accept(serversock);
    send(clientsock, hellomsg, strlen(hellomsg), 0);
    
    /* Clean up. */
    close(serversock); 
    free(data);
}

int main(int argc, char **argv) {
    int opt, isserver, isclient;
    char filepath[1024];

    memset(filepath, '\0', sizeof filepath);
    isserver = 0;
    isclient = 0;

    /* Parse the cmd args. */
    while((opt = getopt(argc, argv, "f:sc:")) != -1) {
        switch(opt) {
            /* File to load or save to. */
            case 'f':
                strncpy(filepath, optarg, sizeof filepath);
                break;
            /* Listen. */
            case 's':
                isserver = 1;
                break;
            /* Client. */
            case 'c':
                isclient = 1;
                break;
            default:
                break;
        }
    }

    /* Chunkasize if were the server and path is set. */
    if(isserver && (filepath[0] != '\0')) {
        server(filepath);
    }
            /* Were a client. */
    else if(isclient && (filepath[0] != '\0')) {

    }
    else {
        fprintf(stderr, "you gave some fucked up params\n");
        return 1;
    }

    return 0;
}
