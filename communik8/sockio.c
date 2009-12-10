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

/* listening socket descriptor. */
int _server_sock;

/* Linked list of socket descriptors. */
struct sock_dec_list {
    int sd;
    struct list_head list;
    void *data;
};

/* Instance of the linked list for read polling. */
struct sock_dec_list _sockreadlist;

struct sock_dec_list _sockwritelist;

/* Function pointers to hook into event system with. */
sio_newcon_fp _newconfp;
sio_read_fp _readfp;
sio_discon_fp _disconfp;

static int setup_socket() {
    int lsock, csockinfosize;
    struct sockaddr_in lsockaddr, csockinfo;
    int on;

    on = 1;

    /* Clear/init some data. */
    memset(&lsockaddr, 0, sizeof(lsockaddr));
    memset(&csockinfo, 0, sizeof(csockinfo));
    csockinfosize = sizeof(csockinfo);

    /* Setup our server socket to a listening state. */
    lsockaddr.sin_family = AF_INET;
    lsockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    lsockaddr.sin_port = htons(13373);

    if((lsock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;
    if(bind(lsock, (struct sockaddr*)&lsockaddr, sizeof(lsockaddr)) < 0)
        return -1;
    if(listen(lsock, 1) < 0)
        return -1;

    /* Set as non blocking so we can select() the mother fucker. */
    ioctl(lsock, FIONBIO, (char *)&on);
    setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    _server_sock = lsock;
    return 0;
}

int sio_setup(sio_newcon_fp nfp, sio_read_fp rdfp, sio_discon_fp dcfp) {
    /* Setup event pointers and socket subsystem. */
    _newconfp = nfp;
    _readfp = rdfp;
    _disconfp = dcfp;
    setup_socket();

    /* Setup linked list for descriptors. */
    INIT_LIST_HEAD(&_sockreadlist.list);
    sio_readlist_add(_server_sock);
    INIT_LIST_HEAD(&_sockwritelist.list);
}

void sio_close() {
    struct sock_dec_list *tmp;
    struct list_head *pos, *q;

    close(_server_sock);
    list_for_each_safe(pos, q, &_sockreadlist.list) {
        tmp = list_entry(pos, struct sock_dec_list, list);
        close(tmp->sd);
        list_del(pos);
        free(tmp); 
    }
    list_for_each_safe(pos, q, &_sockwritelist.list) {
        tmp = list_entry(pos, struct sock_dec_list, list);
        free(tmp->data);
        list_del(pos);
        free(tmp);
    }
}

int sio_readlist_add(int sd) {
    struct sock_dec_list *tmp;
    tmp = malloc(sizeof(*tmp));
    tmp->sd = sd;
    list_add(&(tmp->list), &(_sockreadlist.list));
    printf("descriptor %d added\n", tmp->sd);
    return 0;
}

int sio_writelist_add(int sd, char *buffer) {
    struct sock_dec_list *tmp;
    tmp = malloc(sizeof(*tmp));
    tmp->sd = sd;
    tmp->data = malloc(strlen(buffer) + 1);
    memset(tmp->data, '\0', strlen(buffer)+1);
    memcpy(tmp->data, buffer, strlen(buffer));
    list_add(&(tmp->list), &(_sockwritelist.list));
    printf("message for %d added to write list\n", tmp->sd);
    printf("%s", (char*)tmp->data);
    return 0;
}

int sio_poll() {
    int i, rlen;
    fd_set read_list, write_list;
    struct list_head *pos, *q;
    struct sock_dec_list *tmp;
    //char *readbuff = malloc(1024);
    char readbuff[1024];

    memset(readbuff, '\0', 1024);
    FD_ZERO(&read_list);
    FD_ZERO(&write_list);

    printf("polling IO, read-list: ");
    /* Add entries from list to the bit field. */
    list_for_each(pos, &_sockreadlist.list) {
        tmp = list_entry(pos, struct sock_dec_list, list);
        FD_SET(tmp->sd, &read_list);
        printf("%d ", tmp->sd);
    }
    printf(", write-list: ");
     /* Add messages from write list to the bitfield. */
    list_for_each(pos, &_sockwritelist.list) {
        tmp = list_entry(pos, struct sock_dec_list, list);
        FD_SET(tmp->sd, &write_list);
        printf("%d ", tmp->sd);
    }
    printf("\n");

    /* Do select then step thru list. */
    select(FD_SETSIZE, &read_list, &write_list, NULL, NULL);
    
    /* Step thru the read list and see if their bitfield is marked. */
    list_for_each_safe(pos, q, &_sockreadlist.list) {
        tmp = list_entry(pos, struct sock_dec_list, list);
        /* The server socket. */
        if(tmp->sd == _server_sock && FD_ISSET(tmp->sd, &read_list)) {
            int csock = accept(_server_sock, NULL, 0);
            _newconfp(csock);
            sio_readlist_add(csock);
        }
        /* Clients in the read table. */
        else if(FD_ISSET(tmp->sd, &read_list)) {
            /* Buffer data and fire an event function call. */
            if((rlen = recv(tmp->sd, readbuff, 1023, 0)) != 0) {
                readbuff[rlen+1] = '\0';
                _readfp(tmp->sd, readbuff);
                //free(readbuff);
            }
            /* Discon */
            else {
                /* Free read buffer, close socket, send discon event
                 * remove the descriptor from linked list. */
                //free(readbuff);
                close(tmp->sd);
                _disconfp(tmp->sd);
                list_del(pos);
                free(tmp);
            }
        }
    }

   /* Same for write list. */
   list_for_each_safe(pos, q, &_sockwritelist.list) {
        tmp = list_entry(pos, struct sock_dec_list, list);
        if(FD_ISSET(tmp->sd, &write_list)) {
            /* Send off the message and remove it from the list. */
            printf("sending message to %d\n", tmp->sd);
            send(tmp->sd, tmp->data, strlen(tmp->data), 0);
            free(tmp->data);
            list_del(pos);
            free(tmp);
        }
    }
    return 0;
}
