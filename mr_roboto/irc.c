#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "networking.h"

void send_user(CONNECTION_T *connection) {
    char cmdstr[DEFBUFFSIZE];
    memset(cmdstr, '\0', sizeof cmdstr);
    snprintf(cmdstr, DEFBUFFSIZE, "USER %s %s %s :%s\r\n", connection->config->name, 
                                  "hostname", "servername", connection->config->realname);
    send_string(connection, cmdstr);
}

void send_nick(CONNECTION_T *connection) {
    char cmdstr[DEFBUFFSIZE];
    memset(cmdstr, '\0', sizeof cmdstr);
    snprintf(cmdstr, DEFBUFFSIZE, "NICK %s\r\n", connection->config->name);
    send_string(connection, cmdstr);
}

void send_join(CONNECTION_T *connection, char *arg) {
    char cmdstr[DEFBUFFSIZE];
    memset(cmdstr, '\0', sizeof cmdstr);
    snprintf(cmdstr, DEFBUFFSIZE, "JOIN %s\r\n", arg);
    send_string(connection, cmdstr);
}

void send_part(CONNECTION_T *connection, char *arg) {
    char cmdstr[DEFBUFFSIZE];
    memset(cmdstr,'\0', sizeof cmdstr);
    snprintf(cmdstr, DEFBUFFSIZE, "PART %s\r\n", arg);
    send_string(connection, cmdstr);
}

void send_quit(CONNECTION_T *connection, char *arg) {
    char cmdstr[DEFBUFFSIZE];
    memset(cmdstr, '\0', sizeof cmdstr);
    snprintf(cmdstr, DEFBUFFSIZE, "QUIT %s\r\n", arg);
    send_string(connection, cmdstr);
}

void handle_ping(CONNECTION_T *connection, char *arg) {
    char *cmdstr = calloc(DEFBUFFSIZE, sizeof(char));
    char *origcmdstr = cmdstr;
    char finalstr[DEFBUFFSIZE];

    strncpy(cmdstr, arg, strlen(arg));
    strsep(&cmdstr, " ");
    snprintf(finalstr, DEFBUFFSIZE, "PONG %s\r\n", cmdstr+1);
    fprintf(stderr, "PONG %s\n", cmdstr);

    cmdstr = origcmdstr;
    free(cmdstr);
}
