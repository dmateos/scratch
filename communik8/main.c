#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "sockio.h"
#include "list.h"

int cont = 0;

struct people_list {
    int sd;
    struct list_head list;
} peoples;

void handle_connection(int sd) {
    struct people_list *tmp;
    struct list_head *pos;
    printf("new connection from %d\n", sd);
    
    tmp = malloc(sizeof(*tmp));
    tmp->sd = sd;
    list_add(&(tmp->list), &(peoples.list));

    list_for_each(pos, &peoples.list) {
        tmp = list_entry(pos, struct people_list, list);
        write_list_add(tmp->sd, "new con\n");
    }
    cont++;
}

void handle_read(int sd, char *buffer) {
    struct people_list *tmp;
    struct list_head *pos, *q;
 
    printf("from %d %s", sd, buffer);

    list_for_each_safe(pos, q, &peoples.list) {
        tmp = list_entry(pos, struct people_list, list);
        write_list_add(tmp->sd, buffer);
    }
}

void handle_discon(int sd) {
    struct people_list *tmp;
    struct list_head *pos, *q;

    printf("discon from %d\n", sd);

    list_for_each_safe(pos, q, &peoples.list) {
        tmp = list_entry(pos, struct people_list, list);
        if(tmp->sd == sd) {
            list_del(pos);
            free(tmp);
        }
    }

    list_for_each(pos, &peoples.list) {
        tmp = list_entry(pos, struct people_list, list);
        write_list_add(tmp->sd, "discon\n");
    }
    cont--;
}

int main(int argc, char *argv[]) {
    if(setup_socket_io(handle_connection, handle_read, handle_discon) == -1)
        exit(1);
    
    INIT_LIST_HEAD(&peoples.list);
    
    do {
        poll_io();
    } while(cont > 0);
    
    free_socket_io();
    printf("exiting\n");
    return 0;
}

