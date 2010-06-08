#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "conf.h"
#include "networking.h"
#include "irc.h"

int main() {
    CONFIG_T config;
    CONNECTION_T connection;

    /* Load and check config details. */
    if(load_config("roboto.conf", &config) == -1)
        return -1;
    check_config(&config);

    establish_connection(&config, &connection);
    event_loop(&connection);

    close_connection(&connection);
    free_config(&config);
    return 0;
}
