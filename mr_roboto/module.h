#ifndef _MODULE_H_
#define _MODULE_H_

#include <dlfcn.h>

void *load_module(char *path);
void unload_module(void *module);

#endif
