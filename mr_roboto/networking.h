/* 
 * Handles low level socket connection to a IRC server or any server
 * that sends data in chunks delimited by a certain combo of chars.
 * Note: Handles the rebuilding of fragmented recv()'s 
 */
#ifndef _NETWORKING_H_
#define _NETWORKING_H_

#include "conf.h"

/* Default buffer size for various caches, pretty well over anything
 * to be expected for IRC (4096). */
#define DEFBUFFSIZE 4096

/* Holds low level socket info, counters, and a config object. */
typedef struct CONNECTION_S {
    CONFIG_T *config;
    int socketdesc;
    int reccount, sendcount;
} CONNECTION_T; 

/* Establishes a connection with the server details specified in config, and 
 * builds a connection structure.
 * @param config, Connection config info, pointer kept in new connection struct.
 * @param connection, Structure to store the connection info.
 * @return 0,1
 */
int establish_connection(CONFIG_T *config, CONNECTION_T *connection);

/* Closes the connection created with establish_connection.
 * @param connection, Connection to close. 
 */
void close_connection(CONNECTION_T *connection);

/* Sends a string over the connection specified. 
 * @param connection, Connection to use. 
 * @param msg, String of data to send. 
 * @return, count of data sent.
 */
int send_string(CONNECTION_T *connection, char *msg);

/* Reads data from the connection and passes it to a event reciever to handle. 
 * @param connection, Connection to poll for data. 
 * @param eventptr, Callback to send the data too. 
 */
void event_loop(CONNECTION_T *connection, void(*eventptr)(CONNECTION_T*, char*));

#endif
