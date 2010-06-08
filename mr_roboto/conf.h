#ifndef _CONFIG_H_
#define _CONFIG_H_

#define CFG_BUFFSIZE 1024

typedef struct CONFIG_S {
    char *server;
    char *name;
    char *altname;
    char *realname;
    int port;
    char *cmdchan;
} CONFIG_T;

int load_config(char *path, CONFIG_T *config);
void free_config(CONFIG_T *config);
void check_config(CONFIG_T *config);

#endif
