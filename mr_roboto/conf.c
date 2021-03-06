#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conf.h"

int load_config(char *path, config_t *config) {
    FILE *file;
    char *buffer, *origbuffer;
    char *cmd;

    /* Allocate buffer for lines and set a backup pointer as strsep fucks with it. */
    buffer = calloc(CFG_BUFFSIZE, sizeof(char));
    origbuffer = buffer;
    memset(config, '\0', sizeof(*config));

    if((file = fopen(path, "r")) == NULL) {
        fprintf(stderr, "could not open config %s\n", path);
        return -1;
    }

    config->cfgname = calloc(strlen(path)+1, sizeof(char));
    memcpy(config->cfgname, path, strlen(path));

    /* While not at end of file. */
    while(!feof(file)) {
        /* Read a line and tokenize it on space. */
        memset(buffer, '\0', sizeof(buffer));
        fgets(buffer, CFG_BUFFSIZE, file);
        cmd = strsep(&buffer, " ");

        if(buffer != NULL) {
            /* Remove new lines. */
            if(buffer[strlen(buffer)-1] == '\n') buffer[strlen(buffer)-1] = '\0';

            /* Check the command and copy the arg. */
            if(!strcmp(cmd, "server")) {
                config->server = calloc(strlen(buffer)+1, sizeof(char));
                strncpy(config->server, buffer, strlen(buffer));
            }
            else if(!strcmp(cmd, "port")) {
                config->port = atoi(buffer);
            }
            else if(!strcmp(cmd, "name")) {
                config->name = calloc(strlen(buffer)+1, sizeof(char));
                strncpy(config->name, buffer, strlen(buffer));
            }
            else if(!strcmp(cmd, "altname")) {
                config->altname = calloc(strlen(buffer)+1, sizeof(char));
                strncpy(config->altname, buffer, strlen(buffer));
            }
            else if(!strcmp(cmd, "realname")) {
                config->realname = calloc(strlen(buffer)+1, sizeof(char));
                strncpy(config->realname, buffer, strlen(buffer));
            }
            else if(!strcmp(cmd, "cmdchan")) {
                config->cmdchan = calloc(strlen(buffer)+1, sizeof(char));
                strncpy(config->cmdchan, buffer, strlen(buffer));
            }
            else if(!strcmp(cmd, "module")) {
                if(config->modcount < CFG_MODMAX) {
                    config->modpath[config->modcount] = calloc(strlen(buffer)+1, sizeof(char));
                    strncpy(config->modpath[config->modcount], buffer, strlen(buffer));
                    config->modcount++;
                }
                else {
                    fprintf(stderr, "warning: could not load mod %s due to modmax %d\n", buffer, CFG_MODMAX);
                }
            }
        }
        buffer = origbuffer;
    }
    fclose(file);
    free(buffer);
    return 0;
}

void free_config(config_t *config) {
    int i;

    if(config->server != NULL)
        free(config->server);
    if(config->name != NULL)
        free(config->name);
    if(config->altname != NULL)
        free(config->altname);
    if(config->realname != NULL)
        free(config->realname);
    if(config->cmdchan != NULL)
        free(config->cmdchan);

    for(i = 0; i < config->modcount; i++) {
        if(config->modpath[i] != NULL)
            free(config->modpath[i]);
    }
    free(config->cfgname);
}

void check_config(config_t *config) {
    int i;
    fprintf(stderr, "-----%s------\n", config->cfgname);
    fprintf(stderr, "server: %s\n", config->server);
    fprintf(stderr, "port: %d\n", config->port);
    fprintf(stderr, "name: %s\n", config->name);
    fprintf(stderr, "altname: %s\n", config->altname);
    fprintf(stderr, "realname: %s\n", config->realname);
    fprintf(stderr, "cmdchan: %s\n", config->cmdchan);
    
    for(i = 0; i < config->modcount; i++) {
        fprintf(stderr, "module: %s\n", config->modpath[i]);
    }
    fprintf(stderr, "-----------------\n");
}
