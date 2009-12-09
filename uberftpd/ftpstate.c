#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ftpstate.h"

enum FTPCMDS {
    USER, PASS, ACCT, CWD,
    CDUP, SMNT, QUIT, REIN,
    PORT, PASV, TYPE, STRU,
    MODE, RETR, STOR, STOU,
    APPE, ALLO, REST, RNFR,
    RNTO, ABOR, DELE, RMD,
    MKD, PWD, LIST, NLST,
    SITE, SYST, STAT, HELP,
    NOOP,
};

enum states {
    PREAUTH,
    HAVEUSER,
    PROCESSING,
    ERROR,
    INVALIDUSR,
    EXIT,
};

struct ftp_action_node {
    char state;
    char cmdstr[5];
    char succeed, fail;
    int (*action)(int sock);
};

struct ftp_action_node action_table[10];
int curstate;

static int chkcmd(int command, char *str) {
    if(strcmp(str, action_table[command].cmdstr) == 0)
        return 1;
    else return 0;
}

static void add_action_node(int action, int state, char *cmdstr, char succeed,
                            char fail, int (*actionptr)(int sock)) {
    strncpy(action_table[action].cmdstr, cmdstr, 5);
    action_table[action].state = state;
    action_table[action].succeed = succeed;
    action_table[action].fail = fail;
    action_table[action].action = actionptr;
}

static void run_state(int state, int sock) {
    int var;
    var = action_table[state].action(sock);
    if(var == 0)
        curstate = action_table[state].succeed;
    else
        curstate = action_table[state].fail;
}

static int ACTION_USER(int sock) {
    send(sock, "331\n", strlen("331\n"), 0);
    return 0;
}

static int ACTION_QUIT(int sock) {
    send(sock, "221 bye\n", strlen("221 bye\n"), 0);
    close(sock);
    exit(0);
    return 0;
}

static int ACTION_PASS(int sock) {

}

void ftp_state_init() {
    add_action_node(USER, PREAUTH, "USER", HAVEUSER, ERROR, ACTION_USER);
    add_action_node(PASS, HAVEUSER, "PASS", PROCESSING, INVALIDUSR, ACTION_PASS);

    add_action_node(QUIT, PREAUTH, "QUIT", EXIT, EXIT, ACTION_QUIT);
}

void ftp_proc_command(char *command, int sock) {
    char *cmdp;
    char cmdbuff[1024];

    /* Use a local stack copy of the var as strtok
       is fairly destructive. */
    memcpy(cmdbuff, command, sizeof(cmdbuff));
    cmdp = strtok(cmdbuff, " \n");
    
    printf("cmd is %s, token is %s\n", command, cmdp);
    switch(curstate) {
        /* Commands ran in preauth state. */
        case PREAUTH:
            if(chkcmd(USER, cmdp))
                run_state(USER, sock);
        case HAVEUSER:
            if(chkcmd(PASS, cmdp))
                run_state(PASS, sock);
        default:
            if (chkcmd(QUIT, cmdp))
                run_state(QUIT, sock);
            break;
    }
}
