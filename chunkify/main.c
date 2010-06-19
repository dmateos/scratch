#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "chunkify.h"
#include "networking.h"

static void server(const char *filepath) {
    chunk_t *data;
    int serversock, clientsock, chunkc, request;
    char hellomsg[1024], recbuff[1024];

    /* Chunkify the data and prep a hello message. */
    printf("%s\n", filepath);
    data = chunkify(filepath, &chunkc);
    memset(hellomsg, '\0', sizeof hellomsg);
    snprintf(hellomsg, 1024, "chunkify %d,%d\n", chunkc, CHUNKSIZE);

    /* Accept a client. */
    serversock = socket_listen();

    while(1) {
        clientsock = socket_accept(serversock);
        send(clientsock, hellomsg, strlen(hellomsg), 0);

        memset(recbuff, '\0', sizeof recbuff);
        while(recv(clientsock, recbuff, 1024, 0) > 0) {
            fprintf(stdout, "new data %s", recbuff);
            request = atoi(recbuff);
            if(request < chunkc) {
                fprintf(stdout, "sending chunk %d\n", request);
                send(clientsock, data[request].data, CHUNKSIZE, 0);
            }
            else {
                fprintf(stderr, "chunk over max %d/%d\n", request, chunkc);
                send(clientsock, "chunk-max\n", strlen("chunk-max\n"), 0);
            }
        memset(recbuff, '\0', sizeof recbuff);
        }
    }
    close(serversock);
    /* Clean up. */
    free(data);
}

static void client(const char *hostname, const char *filepath) {
    int servercon;

    fprintf(stdout, "connecting to %s\n", hostname);
    servercon = socket_connect(hostname);
    
}

int main(int argc, char **argv) {
    int opt, isserver, isclient;
    char filepath[1024], servercon[1024];

    memset(filepath, '\0', sizeof filepath);
    memset(servercon, '\0', sizeof servercon);
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
                strncpy(servercon, optarg, sizeof servercon);
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
    else if(isclient && (filepath[0] != '\0' && servercon[0] != '\0')) {
        client(servercon, filepath);
    }
    else {
        fprintf(stderr, "you gave some fucked up params\n");
        return 1;
    }

    return 0;
}
