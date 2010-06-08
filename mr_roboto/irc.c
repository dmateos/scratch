#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "networking.h"

#define IRC_MAXBUFF 3072

void send_user(CONNECTION_T *connection) {
    char cmdstr[IRC_MAXBUFF];
    memset(cmdstr, '\0', sizeof cmdstr);
    snprintf(cmdstr, IRC_MAXBUFF, "USER %s %s %s :%s\r\n", connection->config->name, 
                                  "hostname", "servername", connection->config->realname);
    send_string(connection, cmdstr);
}

void send_nick(CONNECTION_T *connection) {
    char cmdstr[IRC_MAXBUFF];
    memset(cmdstr, '\0', sizeof cmdstr);
    snprintf(cmdstr, IRC_MAXBUFF, "NICK %s\r\n", connection->config->name);
    send_string(connection, cmdstr);
}

void send_join(CONNECTION_T *connection, char *arg) {
    char cmdstr[IRC_MAXBUFF];
    memset(cmdstr, '\0', sizeof cmdstr);
    snprintf(cmdstr, IRC_MAXBUFF, "JOIN %s\r\n", arg);
    send_string(connection, cmdstr);
}

void send_part(CONNECTION_T *connection, char *arg) {
    char cmdstr[IRC_MAXBUFF];
    memset(cmdstr,'\0', sizeof cmdstr);
    snprintf(cmdstr, IRC_MAXBUFF, "PART %s\r\n", arg);
    send_string(connection, cmdstr);
}

void handle_ping(CONNECTION_T *connection, char *arg) {
    char *cmdstr = calloc(IRC_MAXBUFF, sizeof(char));
    char *origcmdstr = cmdstr;
    char finalstr[IRC_MAXBUFF];

    strncpy(cmdstr, arg, strlen(arg));
    strsep(&cmdstr, " ");
    snprintf(finalstr, IRC_MAXBUFF, "PONG %s\r\n", cmdstr+1);
    fprintf(stderr, "PONG %s\n", cmdstr+1);

    cmdstr = origcmdstr;
    free(cmdstr);
}
