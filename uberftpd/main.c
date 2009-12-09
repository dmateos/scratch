#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>


#include "ftpstate.h"

void handle_client(int csock, struct sockaddr_in *csockinfo) {
    char buffer[1024];
    while(1) {
        memset(buffer, '\0', 1024);
        recv(csock, buffer, 1024, 0);
        ftp_proc_command(buffer, csock);
    }
}

int listener() {
    int lsock, csock, csockinfosize, pid;
    struct sockaddr_in lsockaddr, csockinfo;

    /* Clear/init some data. */
    memset(&lsockaddr, 0, sizeof(lsockaddr));
    memset(&csockinfo, 0, sizeof(csockinfo));
    csockinfosize = sizeof(csockinfo);

    /* Setup our server socket to a listening state. */
    lsockaddr.sin_family = AF_INET;
    lsockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    lsockaddr.sin_port = htons(2100);

    if((lsock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        exit(1);
    }
    if(bind(lsock, (struct sockaddr*)&lsockaddr, sizeof(lsockaddr)) < 0) {
        exit(1);
    }
    if(listen(lsock, 1) < 0) {
        exit(1);
    }

    ftp_state_init();

    /* Accept/child fork loop. */
    for(;;) {
        csock = accept(lsock, (struct sockaddr*)&csockinfo, &csockinfosize);
        pid = fork();

        /* Child */
        if(pid == 0) {
            handle_client(csock, &csockinfo);
            close(csock);
            exit(0);
        }
        /* Parent. */
        else {
            close(csock);
        }
    } 
    return 0;
}

int main(int argc, char *argv[]) {
    listener();
    return 0;
}
