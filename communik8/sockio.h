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

#include "list.h"

/* Function ptr types. */
typedef void(*sio_newcon_fp)(int sd);
typedef void(*sio_read_fp)(int sd, char *buffer);
typedef void(*sio_discon_fp)(int sd);

struct sio_socket {
    /* Linked list for read list of sockets to poll. */
    struct sockreadlist {
        int sd;
        struct list_head list;
        struct sockaddr_in saddrs;
    } _sockreadlist;

    /* Linked list for write queue of messages to send to sockets. */
    struct sockwritelist {
        int sd;
        struct list_head list;
        void *data;
    } _sockwritelist;

    /* listening socket descriptor. */
    int _server_sock;
    int _port;

    /* Function pointers to hook into event system with. */
    sio_newcon_fp _newconfp;
    sio_read_fp _readfp;
    sio_discon_fp _disconfp;
};

/* Setup the socket IO system with a non blocking listening socket. */
struct sio_socket *sio_setup(int port, sio_newcon_fp nfp, sio_read_fp rdfp, sio_discon_fp dcfp);

void sio_close(struct sio_socket *socket);

/* Adds to the list of socket descriptors to poll for input. */
int sio_readlist_add(struct sio_socket *socket, int sd, struct sockaddr_in *saddr);

/* Remove descriptor from the read list. */
int sio_readlist_del(struct sio_socket *socket, int sd);

/* Add a message and socket to the write buffer to be sent off 
   when possbile. */
int sio_writelist_add(struct sio_socket *socket, int sd, const char *buffer);

/* Poll socket descriptors for i/o. */
int sio_poll(struct sio_socket *socket);

#endif
