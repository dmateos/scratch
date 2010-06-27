#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "module.h"

void *load_module(char *path) {
    void *module;
    void (*initfp)(void);

    if(!(module = dlopen(path, RTLD_NOW))) {
        fprintf(stderr, "could not load moudle %s\n", path);
        exit(1);
    }

    if(!(initfp = dlsym(module, "mod_main"))) {
        fprintf(stderr, "could not load module init function %s\n", path);
        exit(1);
    }

    return module;
}

void unload_module(void *module) {
    if(module)
        dlclose(module);
}
