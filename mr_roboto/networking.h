#ifndef _NETWORKING_H_
#define _NETWORKING_H_

#include "conf.h"

#define DEFBUFFSIZE 4096

typedef struct CONNECTION_S {
    CONFIG_T *config;
    int socketdesc;
    int reccount, sendcount;
} CONNECTION_T; 

int establish_connection(CONFIG_T *config, CONNECTION_T *connection);
void close_connection(CONNECTION_T *connection);
int send_string(CONNECTION_T *connection, char *msg);
void event_loop(CONNECTION_T *connection, void(*eventptr)(CONNECTION_T*, char*));
void handle_ping(CONNECTION_T *connection, char *arg);
#endif
