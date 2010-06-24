#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "networking.h"
#include "irc.h"

void send_user(connection_t *connection) {
    char cmdstr[DEFBUFFSIZE];
    memset(cmdstr, '\0', sizeof cmdstr);
    snprintf(cmdstr, DEFBUFFSIZE, "USER %s %s %s :%s\r\n", connection->config->name, 
                                  "hostname", "servername", connection->config->realname);
    send_string(connection, cmdstr);
}

void send_nick(connection_t *connection) {
    char cmdstr[DEFBUFFSIZE];
    memset(cmdstr, '\0', sizeof cmdstr);
    snprintf(cmdstr, DEFBUFFSIZE, "NICK %s\r\n", connection->config->name);
    send_string(connection, cmdstr);
}

void send_altnick(connection_t *connection) {
    char cmdstr[DEFBUFFSIZE];
    memset(cmdstr, '\0', sizeof cmdstr);
    snprintf(cmdstr, DEFBUFFSIZE, "NICK %s\r\n", connection->config->altname);
    send_string(connection, cmdstr);
}

void send_join(connection_t *connection, char *arg) {
    char cmdstr[DEFBUFFSIZE];
    memset(cmdstr, '\0', sizeof cmdstr);
    snprintf(cmdstr, DEFBUFFSIZE, "JOIN %s\r\n", arg);
    send_string(connection, cmdstr);
}

void send_part(connection_t *connection, char *arg) {
    char cmdstr[DEFBUFFSIZE];
    memset(cmdstr,'\0', sizeof cmdstr);
    snprintf(cmdstr, DEFBUFFSIZE, "PART %s\r\n", arg);
    send_string(connection, cmdstr);
}

void send_quit(connection_t *connection, char *arg) {
    char cmdstr[DEFBUFFSIZE];
    memset(cmdstr, '\0', sizeof cmdstr);
    snprintf(cmdstr, DEFBUFFSIZE, "QUIT :%s\r\n", arg);
    send_string(connection, cmdstr);
}

void send_mesg(connection_t *connection, char *to, char *arg) {
    char cmdstr[DEFBUFFSIZE];
    memset(cmdstr, '\0', sizeof cmdstr);
    snprintf(cmdstr, DEFBUFFSIZE, "PRIVMSG %s :%s", to, arg);
    send_string(connection, cmdstr);
}

/* Handler functions, parse commands broken up by the main parser. */
void handle_ping(connection_t *connection, char *arg) {
    char *cmdstr = calloc(DEFBUFFSIZE, sizeof(char));
    char *origcmdstr = cmdstr;
    char finalstr[DEFBUFFSIZE];

    strncpy(cmdstr, arg, strlen(arg));
    strsep(&cmdstr, " ");
    snprintf(finalstr, DEFBUFFSIZE, "PONG %s\r\n", cmdstr);
    send_string(connection, finalstr);

    cmdstr = origcmdstr;
    free(cmdstr);
}

static void handle_privmsg(connection_t *connection, ircdata_t *data) {
    
}

static void handle_notice(connection_t *connection, ircdata_t *data) {

}

/* Handles numeric based commands, all in one as theres a shit ton. */
static void handle_numeric(connection_t *connection, ircdata_t *data) {
    int cmd = atoi(data->command);

    switch(cmd) {
        case RPL_NAMEREPLY:
        case RPL_ENDOFNAMES:
        case RPL_MOTD:
        case RPL_MOTDSTART:
        case RPL_ENDOFMOTD:
            break;
        case ERR_NOSUCHNICK:
        case ERR_CANNOTSENDTOCHAN:
        case ERR_UNKNOWNCOMMAND:
        case ERR_NOMOTD:
        case ERR_NONICKNAMEGIVEN:
            break;
        case ERR_NICKNAMEINUSE:
            fprintf(stderr, "nickname in use\n");
            send_altnick(connection);
            break;
        case ERR_NOTONCHANNEL:
        case ERR_NOTREGISTERED:
        case ERR_NEEDMOREPARAMS:
            break;
        default:
            break;
    }
}

void irc_parser(connection_t *connection, char *msg) {
    char *strptr, *backupmsg;
    ircdata_t data;

    /* Ping, easy. */
    if(strstr(msg, "PING :") != NULL) {
        handle_ping(connection, msg);
        return;
    }

    /* MSG: :prefix!prefixend command :params 
       Get the first token, this fucks our msg so make a backup. */
    backupmsg = calloc(strlen(msg)+1, sizeof(char));
    strncpy(backupmsg, msg, strlen(msg));
    if(!(strptr = strtok(msg, " "))) {
        fprintf(stderr, "parse error prefix\n");
        return; /* oh shit. */
    }
     /* Is a prefix if the first char is :. */
    if(strptr[0] == ':') {
        strptr++;
        data.prefix = calloc(strlen(strptr)+1, sizeof(char));
        strncpy(data.prefix, strptr, strlen(strptr));
    }

    /* Grab the next token which should be the command. */
    if(!(strptr = strtok(NULL, " "))) {
        fprintf(stderr, "parse error command\n");
        return; /* more oh shit. */
    }
    data.command = calloc(strlen(strptr)+1, sizeof(char));
    strncpy(data.command, strptr, strlen(strptr));

    /* Seek to the params in our backup msg using the command as reference. */
    if(!(strptr = strstr(backupmsg, data.command)+strlen(data.command)+1)) {
        fprintf(stderr, "parse error params\n");
        return;
    }
    data.params = calloc(strlen(strptr)+1, sizeof(char));
    strncpy(data.params, strptr, strlen(strptr));

    /* First level of data parsed, call subparser depending on the command. */
    fprintf(stderr, "prfx(%s), cmd(%s) %s\n", data.prefix, data.command, data.params);
    if(!strcmp(data.command, "PRIVMSG"))
        handle_privmsg(connection, &data);
    else if(!strcmp(data.command, "NOTICE"))
        handle_notice(connection, &data);
    /* If its a digit we have a numeric command. */
    else if(isdigit(data.command[0]))
        handle_numeric(connection, &data);

    free(backupmsg);
    free(data.prefix);
    free(data.command);
    free(data.params);
}
