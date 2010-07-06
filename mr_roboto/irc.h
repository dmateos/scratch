/* 
 * Parses Lines of the IRC protocol and does .. <abstraction level?>/interface *
 */
#ifndef _IRC_H_
#define _IRC_H_

#include "networking.h"

/* Some quick string tests. */
#define ischan(x) (x[0] == '#')
#define isnumeric(x) (isdigit(x[0]) && isdigit(x[1]) && isdigit(x[2]))
#define isctcp(x) (x[0] == 1 && x[strlen(x)-1] == 1)

/* IRC RFC CMD CODES, not all of them just the ones that look important and added
   as needed. */
#define RPL_NAMEREPLY 353
#define RPL_ENDOFNAMES 366
#define RPL_MOTD 372
#define RPL_MOTDSTART 375
#define RPL_ENDOFMOTD 376

#define ERR_NOSUCHNICK 401
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_UNKNOWNCOMMAND 421
#define ERR_NOMOTD 422
#define ERR_NONICKNAMEGIVEN 431
#define ERR_NICKNAMEINUSE 433
#define ERR_NOTONCHANNEL 443
#define ERR_NOTREGISTERED 451
#define ERR_NEEDMOREPARAMS 461

typedef struct IRC_DATA_S {
    char *prefix;
    char *command;
    char *params;
} ircdata_t;

typedef void(*privmsg_cbt)(connection_t *connection, char *to, char *from, char *fromhost, char *msg);
extern privmsg_cbt privmsg_cb;

void irc_parser(connection_t *connection, char *msg);
#endif
