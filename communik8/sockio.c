/* 
 * Synchronus IO multiplexing with an event system.
 * Daniel Mateos, Dec 11th, 2009
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "list.h" /* Linux kernel linked list implementation.*/
#include "sockio.h"
#include "util.h" 

#define BUFSIZE 1024

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

/* Function pointers to hook into event system with. */
sio_newcon_fp _newconfp;
sio_read_fp _readfp;
sio_discon_fp _disconfp;

/*
 * Sets up a standard TCP listening socket. 
 */
static int setup_socket(int port) {
    int lsock, csockinfosize;
    struct sockaddr_in lsockaddr, csockinfo;
    char on;

    /* Clear/init some data. */
    on = 1;
    memset(&lsockaddr, 0, sizeof(lsockaddr));
    memset(&csockinfo, 0, sizeof(csockinfo));
    csockinfosize = sizeof(csockinfo);

    /* Setup our server socket to a listening state. */
    lsockaddr.sin_family = AF_INET;
    lsockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    lsockaddr.sin_port = htons(port);

    if((lsock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;
    if(bind(lsock, (struct sockaddr*)&lsockaddr, sizeof(lsockaddr)) < 0)
        return -1;
    if(listen(lsock, 1) < 0)
        return -1;

    /* Set as non blocking so we can select() the mother fucker. */
    ioctl(lsock, FIONBIO, &on);
    setsockopt(lsock, SOL_IP, SO_REUSEADDR, &on, sizeof(char));

    printf("listening on tcp port %d\n", port);
    return lsock;
}

int sio_setup(sio_newcon_fp nfp, sio_read_fp rdfp, sio_discon_fp dcfp) {
    /* Setup event pointers and socket subsystem. */
    _newconfp = nfp;
    _readfp = rdfp;
    _disconfp = dcfp;
    if((_server_sock = setup_socket(3141)) < 0)
        return -1;

    /* Setup linked list for descriptors. */
    INIT_LIST_HEAD(&_sockreadlist.list);
    sio_readlist_add(_server_sock, NULL);
    INIT_LIST_HEAD(&_sockwritelist.list);
    return 0;
}

void sio_close() {
    struct sockreadlist *rtmp;
    struct sockwritelist *wtmp;
    struct list_head *pos, *q;

    /* Close server socket and clean out the lists of anything
     * left allocated. */
    close(_server_sock);
    list_for_each_safe(pos, q, &_sockreadlist.list) {
        rtmp = list_entry(pos, struct sockreadlist, list);
        close(rtmp->sd);
        list_del(pos);
        efree(rtmp); 
    }
    list_for_each_safe(pos, q, &_sockwritelist.list) {
        wtmp = list_entry(pos, struct sockwritelist, list);
        efree(wtmp->data);
        list_del(pos);
        efree(wtmp);
    }
}

int sio_readlist_add(int sd, struct sockaddr_in *saddr) {
    struct sockreadlist *tmp;

    tmp = emalloc(sizeof(*tmp));
    tmp->sd = sd;
    if(saddr != NULL)
        memcpy(&tmp->saddrs, saddr, sizeof(tmp->saddrs));
    list_add(&(tmp->list), &(_sockreadlist.list));
    printf("descriptor %d added\n", tmp->sd);

    return 0;
}

int sio_readlist_remove(int sd) {
    struct sockreadlist *tmp;
    struct list_head *prev, *q;

    list_for_each_safe(prev, q, &_sockreadlist.list) {
        tmp = list_entry(pos, struct sockreadlist, list);
        close(tmp->sd);
        list_del(pos);
        efree(rtmp);
    }
   return 0;
}

int sio_writelist_add(int sd, const char *buffer) {
    struct sockwritelist *tmp;

    tmp = emalloc(sizeof(*tmp));
    tmp->sd = sd;
    tmp->data = emalloc(strlen(buffer) + 1);
    memset(tmp->data, '\0', strlen(buffer)+1);
    memcpy(tmp->data, buffer, strlen(buffer));
    list_add(&(tmp->list), &(_sockwritelist.list));
    printf("message for %d added to write list\n", tmp->sd);
    printf("\t%s", (char*)tmp->data);

    return 0;
}

int sio_poll() {
    int i, rlen;
    fd_set read_list, write_list;
    struct list_head *pos, *q;
    struct sockreadlist *rtmp;
    struct sockwritelist *wtmp;
    char readbuff[BUFSIZE];
    struct sockaddr_in cinfo;
    socklen_t cinfolen;

    memset(readbuff, '\0', BUFSIZE);
    FD_ZERO(&read_list);
    FD_ZERO(&write_list);
    cinfolen = sizeof(cinfo);

    printf("polling IO, read-list: ");
    /* Add entries from list to the bit field. */
    list_for_each(pos, &_sockreadlist.list) {
        rtmp = list_entry(pos, struct sockreadlist, list);
        FD_SET(rtmp->sd, &read_list);
        printf("%d ", rtmp->sd);
    }
    printf(", write-list: ");
     /* Add messages from write list to the bitfield. */
    list_for_each(pos, &_sockwritelist.list) {
        wtmp = list_entry(pos, struct sockwritelist, list);
        FD_SET(wtmp->sd, &write_list);
        printf("%d ", wtmp->sd);
    }
    printf("\n");

    /* Do select then step thru list. */
    select(FD_SETSIZE, &read_list, &write_list, NULL, NULL);
    
    /* Step thru the read list and see if their bitfield is marked. */
    list_for_each_safe(pos, q, &_sockreadlist.list) {
        rtmp = list_entry(pos, struct sockreadlist, list);
        /* The server socket. */
        if(rtmp->sd == _server_sock && FD_ISSET(rtmp->sd, &read_list)) {
            int csock = accept(_server_sock, (struct sockaddr*)&cinfo, &cinfolen);
            _newconfp(csock);
            sio_readlist_add(csock, &cinfo);
        }
        /* Clients in the read table. */
        else if(FD_ISSET(rtmp->sd, &read_list)) {
            /* Buffer data and fire an event function call. */
            if((rlen = recv(rtmp->sd, readbuff, BUFSIZE-1, 0)) != 0) {
                readbuff[rlen+1] = '\0';
                _readfp(rtmp->sd, readbuff);
            }
            /* Discon */
            else {
                /* Free read buffer, close socket, send discon event
                 * remove the descriptor from linked list. */
                close(rtmp->sd);
                _disconfp(rtmp->sd);
                list_del(pos);
                efree(rtmp);
            }
        }
    }

   /* Same for write list. */
   list_for_each_safe(pos, q, &_sockwritelist.list) {
        wtmp = list_entry(pos, struct sockwritelist, list);
        if(FD_ISSET(wtmp->sd, &write_list)) {
            /* Send off the message and remove it from the list. */
            printf("sending message to %d\n", wtmp->sd);
            send(wtmp->sd, wtmp->data, strlen(wtmp->data), 0);
            efree(wtmp->data);
            list_del(pos);
            efree(wtmp);
        }
    }
    return 0;
}
