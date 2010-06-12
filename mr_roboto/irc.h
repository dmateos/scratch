#ifndef _IRC_H_
#define _IRC_H_

#include "networking.h"

typedef struct IRC_DATA_S {
    char *prefix;
    char *command;
    char *params;
} IRCDATA_T;

void send_user(CONNECTION_T *connection);
void send_nick(CONNECTION_T *connection);
void send_join(CONNECTION_T *connection, char *arg);
void send_part(CONNECTION_T *connection, char *arg);
void send_quit(CONNECTION_T *connection, char *arg);
void send_mesg(CONNECTION_T *connection, char *to, char *arg);

void handle_ping(CONNECTION_T *connection, char *arg);
void hamdle_privmsg(CONNECTION_T *connection, IRCDATA_T *data);

void irc_parser(CONNECTION_T *connection, char *msg);
#endif
