/* 
 * Parses Lines of the IRC protocol and does .. <abstraction level?>/interface *
 */
#ifndef _IRC_SEND_H_
#define _IRC_SEND_H_

#include "networking.h"

void send_user(connection_t *connection);
void send_nick(connection_t *connection);
void send_altnick(connection_t *connection);
void send_join(connection_t *connection, char *arg);
void send_part(connection_t *connection, char *arg);
void send_quit(connection_t *connection, char *arg);
void send_mesg(connection_t *connection, char *to, char *arg);
void send_notice(connection_t *connection, char *to, char *arg);
void send_ctcp(connection_t *connection, char *to, char *arg);

#endif
