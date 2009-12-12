/* 
 * Synchronus IO multiplexing with an event system.
 * Daniel Mateos, Dec 11th, 2009
 *
 * This implements a socket server using asyncronus IO via select(), calls
 * are received via subscribing to various events, ie newcon, discon, read, write
 */
#ifndef _SOCKET_IO_H
#define _SOCKET_IO_H

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

/* Function ptr types. */
typedef void(*sio_newcon_fp)(int sd);
typedef void(*sio_read_fp)(int sd, char *buffer);
typedef void(*sio_discon_fp)(int sd);

/* Setup the socket IO system with a non blocking listening socket. */
int sio_setup(sio_newcon_fp nfp, sio_read_fp rdfp, sio_discon_fp dcfp);

void sio_close();

/* Adds to the list of socket descriptors to poll for input. */
int sio_readlist_add(int sd, struct sockaddr_in *saddr);

/* Add a message and socket to the write buffer to be sent off 
   when possbile. */
int sio_writelist_add(int sd, const char *buffer);

/* Poll socket descriptors for i/o. */
int sio_poll();

#endif
