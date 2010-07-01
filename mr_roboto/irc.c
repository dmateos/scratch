#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "networking.h"
#include "irc.h"
#include "irc_send.h"

/* Handler functions, parse commands broken up by the main parser. */
static void handle_ping(connection_t *connection, ircdata_t *data) {
    char cmdstr[DEFBUFFSIZE];
    memset(cmdstr, '\0', sizeof cmdstr);
    snprintf(cmdstr, DEFBUFFSIZE, "PONG %s\r\n", data->params);
    send_string(connection, cmdstr);
}

/* Called by handle_privmsg if its a ctcp msg. */
static void handle_ctcp(connection_t *connection, char *to, char *from, char *fromhost, char *msg) {
    /* Strip the ctcp encaps. */
    msg+=1;
    msg[strlen(msg)-1] = '\0';

    if(!strcasecmp(msg, "VERSION"))
        send_ctcp(connection, from, "VERSION MrRoboto 0.1 (http://github.com/dmateos/scratch/blob/master/mr_roboto/)");
    else if(!strcasecmp(msg, "PING"))
        send_ctcp(connection, from, "PING");
}

static void handle_privmsg(connection_t *connection, ircdata_t *data) {
    char *to, *msg, *from, *fromhost;

    /* Who msg to and the message are in the params. 
       Set to, then seek to the next param chop it off and set it as msg. */
    to = data->params;
    msg = strchr(data->params, ' ');
    *msg = '\0';
    msg+=2;

    /* Who its from is in the prefix. */
    from = data->prefix;
    fromhost = strchr(data->prefix, '!'); /* Could be null if from server? */
    *fromhost = '\0';
    fromhost+=1;

    /* Might be a CTCP privmsg, we handle that elsewhere. */
    if(msg[0] == 1 && msg[strlen(msg)-1] == 1) {
        handle_ctcp(connection, to, from, fromhost, msg);
        return;
    }

    /* Now we can do stuff with the data finally! */
    fprintf(stderr, "\tpm from %s(%s)\n\tto %s\n\t%s\n", from, fromhost, to, msg);

    /*
    if(ischan(to))
        send_mesg(connection, to, msg); //channel echo!
    else 
        send_mesg(connection, from, msg); //echo!
    */
}

static void handle_chanevent(connection_t *connection, ircdata_t *data) {
    
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
    memset(&data, '\0', sizeof(data));    

    /* Get the first token, this fucks the params part so make a backup. */
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
    /* Else its a command. */
    else {
        data.command = calloc(strlen(strptr)+1, sizeof(char));
        strncpy(data.command, strptr, strlen(strptr));
    }

    /* If the above was a prefix (no command), it means NOW we get the comnand. */
    if(data.command == NULL) {
        if(!(strptr = strtok(NULL, " "))) {
            fprintf(stderr, "parse error command\n");
            return; /* more oh shit. */
        }
        data.command = calloc(strlen(strptr)+1, sizeof(char));
        strncpy(data.command, strptr, strlen(strptr));
    }

    /* Seek to the params in our backup msg using the command as reference. */
    if(!(strptr = strstr(backupmsg, data.command)+strlen(data.command)+1)) {
        fprintf(stderr, "parse error params\n");
        return;
    }
    data.params = calloc(strlen(strptr)+1, sizeof(char));
    strncpy(data.params, strptr, strlen(strptr));

    /* First level of data parsed, we now have a structure containing possibly a prefix, 
     * a command and some params, call subparser depending on the command. */
    fprintf(stderr, "prfx(%s), cmd(%s) %s\n", data.prefix, data.command, data.params);
    /* If its a digit we have a numeric command. */
    if(isdigit(data.command[0]))
        handle_numeric(connection, &data);
    else if(!strcmp(data.command, "PING"))
        handle_ping(connection, &data);
    else if(!strcmp(data.command, "PRIVMSG"))
            handle_privmsg(connection, &data);
    else if(!strcmp(data.command, "NOTICE"))
        ;
    else if(!strcmp(data.command, "MODE"))
        handle_chanevent(connection, &data);
    else if(!strcmp(data.command, "JOIN"))
        handle_chanevent(connection, &data);
    else if(!strcmp(data.command, "PART"))
        handle_chanevent(connection, &data);
    else if(!strcmp(data.command, "TOPIC"))
        handle_chanevent(connection, &data);
    else if(!strcmp(data.command, "INVITE"))
        handle_chanevent(connection, &data);
    else if(!strcmp(data.command, "KICK"))
        handle_chanevent(connection, &data);
    else if(!strcmp(data.command, "QUIT"))
        handle_chanevent(connection, &data);

    free(backupmsg);
    if(data.prefix) free(data.prefix);
    free(data.command);
    free(data.params);
}
