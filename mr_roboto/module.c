#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "module.h"

void *load_module(char *path) {
    void *module;
    void (*initfp)(void);

    /* Load the module binary and get its init function. */
    if(!(module = dlopen(path, RTLD_NOW))) {
        fprintf(stderr, "could not load moudle %s\n", path);
        return NULL;
    }
    if(!(initfp = dlsym(module, "mod_main"))) {
        fprintf(stderr, "could not load module init function %s\n", path);
    }

    /* Call the init function. */
    initfp();
    return module;
}

void unload_module(void *module) {
    void (*destructfp)(void);

    /* If a destructor exsists, run it. */
    if((destructfp = dlsym(module, "mod_exit")))
        destructfp();

    dlclose(module);
}
