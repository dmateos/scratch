#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "networking.h"
#include "conf.h"
#include "irc.h"

int establish_connection(CONFIG_T *config, CONNECTION_T *connection) {
    struct sockaddr_in pin;
    struct hostent *hp;
    int sd;

    if((hp = gethostbyname(config->server)) == 0) {
        return -1;
    }

    memset(&pin, '\0', sizeof(pin));
    pin.sin_family = AF_INET;
    pin.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
    pin.sin_port = htons(config->port);

    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return -1;
    }
    if(connect(sd, (struct sockaddr*)&pin, sizeof(pin)) == -1) {
        return -1;
    }

    memset(connection, '\0', sizeof(*connection));
    connection->socketdesc = sd;
    connection->config = config;

    return sd;
}

void close_connection(CONNECTION_T *connection) {
    close(connection->socketdesc);
    fprintf(stderr, "sent:%d bytes, recv:%d bytes\n", connection->sendcount, connection->reccount);
}

int send_string(CONNECTION_T *connection, char *message) {
    int sent;
    sent = send(connection->socketdesc, message, strlen(message), 0);
    connection->sendcount += sent;
    return sent;
}

struct readstate_s { 
    char backbuffer[DEFBUFFSIZE];
    char backbufferisdirty;
    char message[DEFBUFFSIZE*2];
};

static void *read_line(char *buffer, struct readstate_s *state) {
    if(*buffer == '\0')
        return NULL; 

    char *newend = strstr(buffer, "\r\n");
    char *message = state->message;
    memset(message, '\0', DEFBUFFSIZE);

    if(state->backbufferisdirty) {
        fprintf(stderr, "emptying backbuffer (%s)\n", state->backbuffer); 
        memcpy(message, state->backbuffer, strlen(state->backbuffer));
        message += strlen(state->backbuffer);
        state->backbufferisdirty = 0;
        memset(state->backbuffer, '\0', sizeof state->backbuffer);
    }

    /* Crap no end. */
    if(newend == NULL) {
        fprintf(stderr, "filling backbuffer (%s)\n", buffer);
        memcpy(state->backbuffer, buffer, strlen(buffer));        
        state->backbufferisdirty = 1;
        return NULL;
    }

    /* Terminate and copy. */
    *newend = '\0';
    memcpy(message, buffer, strlen(buffer));
    return newend+2;
}

void event_loop(CONNECTION_T *connection) {
    char recbuffer[DEFBUFFSIZE];
    int count = 0;
    char *mptr;
    struct readstate_s readstate;

    memset(&readstate, '\0', sizeof readstate);

    send_user(connection);
    send_nick(connection);
    send_join(connection, connection->config->cmdchan);

    /* Grab some IRC data. */
    do {
        memset(recbuffer, '\0', DEFBUFFSIZE);
        count = recv(connection->socketdesc, recbuffer, DEFBUFFSIZE-1, 0);
        connection->reccount += count;
        mptr = recbuffer;

        /* Parse all the data line by line. */
        printf("----recv block %d/%d-----\n", count, DEFBUFFSIZE);
        while((mptr = read_line(mptr, &readstate))) {
            printf("line: %s\n", readstate.message);

            if(strstr(readstate.message, "PING") != NULL)
                handle_ping(connection, readstate.message);
            else if (strstr(readstate.message, "plzplzquit") != NULL)
                send_quit(connection, "l8r guys");
        }
    } while(count > 0);
}
