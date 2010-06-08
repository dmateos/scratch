#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "conf.h"
#include "networking.h"
#include "irc.h"

int main(int argc, char **argv) {
    CONFIG_T config;
    CONNECTION_T connection;
    char *configp; 

    if(argc == 2 && (strlen(argv[1]) > 0))
        configp = argv[1];
    else
        configp = "roboto.conf";

    /* Load and check config details. */
    if(load_config(configp, &config) == -1)
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
