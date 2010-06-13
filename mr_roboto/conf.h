/*
 * Handles loading configuration data, verifying integrity and loading into 
 * a structure that is easy to deal with. */
#ifndef _CONFIG_H_
#define _CONFIG_H_

#define CFG_BUFFSIZE 1024

/* Structure containing various config entries. */
typedef struct CONFIG_S {
    char *server;
    char *name;
    char *altname;
    char *realname;
    int port;
    char *cmdchan;
    char *cfgname;
} CONFIG_T;

/* Loads a config from a file into the config structure. 
 * @param path, Path of the config file to load.
 * @param config, Config structure to store data in (preallocated with allocated entries) 
 * @returns 0,1
 */
int load_config(char *path, CONFIG_T *config);

/* Frees a configuration files allocated entries.
 *  @param config, The config structure to free.
 */
void free_config(CONFIG_T *config);

/* Checks integrity of config file.
 * @param config, Config object to check. 
 */
void check_config(CONFIG_T *config);

#endif
