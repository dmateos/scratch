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

/*
char *read_line(char **top, char *message, char **remnant) {
    char *newend = strstr(buffer, "\r\n");
    char *backbuff;
    if(newend == NULL) {
        if(*newend != '\0') {
            backbuff = calloc(strlen(buffer+1), sizeof(char));
        }
        return 0;
    }
    *newend = '\0';
    strncpy(message, buffer, 3071);
    return newend+2;
}
*/
void event_loop(CONNECTION_T *connection) {
    char buffer[3071+1];
    char message[3071+1];
    int count = 0;

    send_user(connection);
    send_nick(connection);
    send_join(connection, connection->config->cmdchan);

    /* Grab some IRC data. */
    do {
        memset(buffer, '\0', 3071+1);
        memset(message, '\0', 3071+1);
        count = recv(connection->socketdesc, buffer, 3071, 0);
        connection->reccount += count;
        printf("%s\n", buffer);

        if(strstr(buffer, "PING") == &buffer[0]) {
            handle_ping(connection, buffer);
        }

        /* Parse all the data line by line. */
        //while((mptr = read_line(buffer, message)) != NULL) {
          //  printf("new line: %s\n", message);
        //}
    } while(count != 0 && count != -1);   
}
