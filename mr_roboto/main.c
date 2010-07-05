#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "main.h"
#include "conf.h"
#include "networking.h"
#include "irc.h"
#include "irc_send.h"
#include "module.h"

int main(int argc, char **argv) {
    int opt, i;
    char configpath[128];
    config_t config;
    connection_t connection;
    void *module[CFG_MODMAX];

    memset(configpath, '\0', sizeof configpath);
    strncpy(configpath, "roboto.conf", strlen("roboto.conf"));

    /* Parse command line args. */
    while((opt = getopt(argc, argv, "c:rhv")) != -1) {
        switch(opt) {
            case 'c':
                memset(configpath, '\0', sizeof configpath);
                strncpy(configpath, optarg, sizeof configpath);                
                break;
            /* TODO relay mode? could be cool. */
            case 'r':
                break;
            case 'h':
            case 'v':
                fprintf(stdout, "mrroboto v0.1\n");
                exit(0);
                break;
            default:
                break;
        }
    }

    /* Load and check config details. */
    if(!load_config(configpath, &config))
        check_config(&config);
    else 
        return -1;

    /* Load our c bot modules if any. */
    for(i = 0; i < config.modcount; i++) {
        module[i] = load_module(config.modpath[i]);
    }

    establish_connection(&config, &connection);
    send_user(&connection);
    send_nick(&connection);
    send_join(&connection, config.cmdchan);
    event_loop(&connection, irc_parser);

    for(i = 0; i < config.modcount; i++)
        if(module[i]) unload_module(module[i]);
    close_connection(&connection);
    free_config(&config);

    return 0;
}
