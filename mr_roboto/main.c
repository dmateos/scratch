#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "main.h"
#include "conf.h"
#include "networking.h"
#include "irc.h"

int main(int argc, char **argv) {
    config_t config;
    connection_t connection;
    char configpath[128];
    int opt;

    memset(configpath, '\0', sizeof configpath);
    strncpy(configpath, "roboto.conf", strlen("roboto.conf"));

    while((opt = getopt(argc, argv, "c:")) != -1) {
        switch(opt) {
            case 'c':
                memset(configpath, '\0', sizeof configpath);
                strncpy(configpath, optarg, strlen(optarg));                
                break;
            default:
                break;
        }
    }

    /* Load and check config details. */
    if(load_config(configpath, &config) == -1)
        return -1;
    check_config(&config);

    establish_connection(&config, &connection);
    send_user(&connection);
    send_nick(&connection);
    send_join(&connection, config.cmdchan);
    event_loop(&connection, irc_parser);

    close_connection(&connection);
    free_config(&config);
    return 0;
}
