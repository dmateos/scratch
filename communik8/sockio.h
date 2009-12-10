/* This implements a socket server using asyncronus IO via select(), calls
 * are received via subscribing to various events, ie newcon, discon, read, write
 */
#ifndef _SOCKET_IO_H
#define _SOCKET_IO_H

/* Function ptr types. */
typedef void(*newcon_fp)(int sd);
typedef void(*read_fp)(int sd, char *buffer);
typedef void(*discon_fp)(int sd);

/* Setup the socket IO system with a non blocking listening socket. */
int setup_socket_io(newcon_fp nfp, read_fp rdfp, discon_fp dcfp);

void free_socket_io();

/* Adds to the list of socket descriptors to poll for input. */
int read_list_add(int sd);

/* Add a message and socket to the write buffer to be sent off 
   when possbile. */
int write_list_add(int sd, char *buffer);

/* Poll socket descriptors for i/o. */
int poll_io();

#endif
