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

static void print_help() {
    fprintf(stdout, "Mr Roboto Modular IRC Bot Version 0.1 Alpha1\n");
    fprintf(stdout, "http://github.com/dmateos/scratch/tree/master/mr_roboto/\n");
    fprintf(stdout, "\t-c arg Specify config file (default roboto.conf)\n");
    fprintf(stdout, "\t-d level Debug level (default 0)\n");
    fprintf(stdout, "\t-h Print this help page\n");
    fprintf(stdout, "\t-v Print the program version\n");
}

int main(int argc, char **argv) {
    int opt, i;
    char configpath[128];
    config_t config;
    connection_t connection;
    moddata_t *module[CFG_MODMAX];

    memset(configpath, '\0', sizeof configpath);
    strncpy(configpath, "roboto.conf", strlen("roboto.conf"));

    /* Parse command line args. */
    while((opt = getopt(argc, argv, "c:rhvd")) != -1) {
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
                print_help();
                exit(0);
            case 'd':
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
        fprintf(stderr, "Loading mod: %s\n", config.modpath[i]);
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
