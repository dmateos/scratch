#ifndef _IRC_H_
#define _IRC_H_

#include "networking.h"

void send_user(CONNECTION_T *connection);
void send_nick(CONNECTION_T *connection);
void send_join(CONNECTION_T *connection, char *arg);
void send_part(CONNECTION_T *connection, char *arg);
void send_quit(CONNECTION_T *connection, char *arg);

#endif
