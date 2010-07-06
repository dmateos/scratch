#define IS_MODULE

#include <stdio.h>
#include <string.h>

#include "../module.h"
#include "../irc.h"
#include "../networking.h"
#include "../irc_send.h"

moddata_t *moddatag;

/* Echo */
void handle_privmsg(connection_t *connection, char *to, char *from, char *fromhost, char *msg) {
    if(ischan(to))
        moddatag->send_mesg(connection, to, msg);
    else
        moddatag->send_mesg(connection, from, msg);
}

void mod_main(moddata_t *moddata) {
    moddata->id[0] = 'E';
    moddata->id[1] = 'C';
    moddata->id[2] = 'H';
    moddata->id[3] = 'O';

    moddata->privmsg_cb = handle_privmsg;
    moddatag = moddata;
}

void mod_exit() {
}
