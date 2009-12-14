#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "sockio.h"
#include "list.h"

#define BUFSIZE 1024

int cont = 0;

struct people_list {
    int sd;
    struct list_head list;
} peoples;

struct sio_socket *sock;

void handle_connection(int sd) {
    struct people_list *tmp;
    struct list_head *pos;
    char pbuff[BUFSIZE];

    printf("new connection from %d\n", sd);

    /* Allocate a new person entry and put the socket
     * desc into it. */    
    tmp = malloc(sizeof(*tmp));
    tmp->sd = sd;
    list_add(&(tmp->list), &(peoples.list));

    /* Add to every other person to the write queue telling them
     * this guy has joined. */
    list_for_each(pos, &peoples.list) {
        tmp = list_entry(pos, struct people_list, list);
        snprintf(pbuff, BUFSIZE, "new con from %d\n", sd);
        sio_writelist_add(sock, tmp->sd, pbuff);
    }
    cont++;
}

void handle_read(int sd, char *buffer) {
    struct people_list *tmp;
    struct list_head *pos;
    char pbuff[BUFSIZE];
 
    printf("from %d %s", sd, buffer);

    /* Echo what was said to everyone in the person list. */
    list_for_each(pos, &peoples.list) {
        tmp = list_entry(pos, struct people_list, list);
        snprintf(pbuff, BUFSIZE, "%d said: %s", sd, buffer);
        sio_writelist_add(sock, tmp->sd, pbuff);
    }
}

void handle_discon(int sd) {
    struct people_list *tmp;
    struct list_head *pos, *q;
    char pbuff[BUFSIZE];

    printf("discon from %d\n", sd);

    /* Find them in the person list and remove. */
    list_for_each_safe(pos, q, &peoples.list) {
        tmp = list_entry(pos, struct people_list, list);
        if(tmp->sd == sd) {
            list_del(pos);
            free(tmp);
        }
    }

    /* Tell everyone. */
    list_for_each(pos, &peoples.list) {
        tmp = list_entry(pos, struct people_list, list);
        snprintf(pbuff, BUFSIZE, "discon from %d\n", sd);
        sio_writelist_add(sock, tmp->sd, pbuff);
    }
    cont--;
}

int main(int argc, char *argv[]) {
    if((sock = sio_setup(3141, handle_connection, handle_read, handle_discon)) == NULL)
        exit(1);
    
    INIT_LIST_HEAD(&peoples.list);
    
    do {
        sio_poll(sock);
    } while(cont > 0);
    
    sio_close(sock);
    printf("exiting\n");
    return 0;
}

