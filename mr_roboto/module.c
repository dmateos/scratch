#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "module.h"

#define INITF "mod_main"
#define EXITF "mod_exit"

void *load_module(char *path) {
    void *module;
    void (*initfp)(void);

    /* Load the module binary and get its init function. */
    if(!(module = dlopen(path, RTLD_NOW))) {
        fprintf(stderr, "could not load moudle %s\n", path);
        return NULL;
    }
    if(!(initfp = dlsym(module, INITF))) {
        fprintf(stderr, "could not load module init function %s\n", path);
    }

    /* Call the init function. */
    initfp();
    return module;
}

void unload_module(void *module) {
    void (*destructfp)(void);

    /* If a destructor exsists, run it. */
    if((destructfp = dlsym(module, EXITF)))
        destructfp();

    dlclose(module);
}
