#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "module.h"
#include "irc.h"
#include "irc_send.h"

#define INITF "mod_main"
#define EXITF "mod_exit"

moddata_t *load_module(char *path) {
    void (*initfp)(moddata_t *moddata);
    moddata_t *moddata;

    moddata = calloc(1, sizeof *moddata);

    /* Load the module binary and get its init function. */
    if(!(moddata->modhandle = dlopen(path, RTLD_NOW))) {
        fprintf(stderr, "could not load moudle %s\n", path);
        return NULL;
    }
    if(!(initfp = dlsym(moddata->modhandle, INITF))) {
        fprintf(stderr, "could not load module init function %s\n", path);
    }

    /* API map. */
    moddata->send_mesg = send_mesg;

    /* Call the init function. */
    initfp(moddata);
    fprintf(stderr, "mod id %s\n", moddata->id);

    privmsg_cb = moddata->privmsg_cb; /* Nameing shit and multiplex. */ 
    return moddata;
}

void unload_module(moddata_t *moddata) {
    void (*destructfp)(void);

    /* If a destructor exsists, run it. */
    if((destructfp = dlsym(moddata->modhandle, EXITF)))
        destructfp();

    dlclose(moddata->modhandle);
    free(moddata);
}
