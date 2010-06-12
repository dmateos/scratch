#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "networking.h"
#include "irc.h"

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
    snprintf(cmdstr, DEFBUFFSIZE, "QUIT :%s\r\n", arg);
    send_string(connection, cmdstr);
}

void send_mesg(CONNECTION_T *connection, char *to, char *arg) {
    char cmdstr[DEFBUFFSIZE];
    memset(cmdstr, '\0', sizeof cmdstr);
    snprintf(cmdstr, DEFBUFFSIZE, "PRIVMSG %s :%s", to, arg);
    send_string(connection, cmdstr);
}

void handle_ping(CONNECTION_T *connection, char *arg) {
    char *cmdstr = calloc(DEFBUFFSIZE, sizeof(char));
    char *origcmdstr = cmdstr;
    char finalstr[DEFBUFFSIZE];

    strncpy(cmdstr, arg, strlen(arg));
    strsep(&cmdstr, " ");
    snprintf(finalstr, DEFBUFFSIZE, "PONG %s\r\n", cmdstr);
    send_string(connection, finalstr);

    cmdstr = origcmdstr;
    free(cmdstr);
}

void irc_parser(CONNECTION_T *connection, char *msg) {
    char *strptr;
    char *backupmsg;
    IRCDATA_T data;

    /* Ping, easy. */
    if(strstr(msg, "PING :") != NULL) {
        handle_ping(connection, msg);
        return;
    }

    /* Get the first token, this fucks our msg so make a backup. */
    backupmsg = calloc(strlen(msg)+1, sizeof(char));
    strncpy(backupmsg, msg, strlen(msg));

    if(!(strptr = strtok(msg, " "))) {
        fprintf(stderr, "parse error prefix\n");
        return; /* oh shit. */
    }

     /* Is a prefix if the first char is :. */
    if(strptr[0] == ':') {
        data.prefix = calloc(strlen(strptr)+1, sizeof(char));
        strncpy(data.prefix, strptr, strlen(strptr));
    }

    /* Grab the next token which should be the command. */
    if(!(strptr = strtok(NULL, " "))) {
        fprintf(stderr, "parse error command\n");
        return; /* more oh shit. */
    }
    data.command = calloc(strlen(strptr)+1, sizeof(char));
    strncpy(data.command, strptr, strlen(strptr));

    /* Seek to the params in our backup msg using the command as reference. */
    strptr = strstr(backupmsg, data.command)+strlen(data.command)+1;
    data.params = calloc(strlen(strptr)+1, sizeof(char));
    strncpy(data.params, strptr, strlen(strptr));

    fprintf(stderr, "prefix: %s, cmd: %s\nparams: %s\n\n", data.prefix, data.command, data.params);
    free(backupmsg);

    /* temp to test valgrind. */
    free(data.prefix);
    free(data.command);
    free(data.params);
}
