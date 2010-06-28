#ifndef _MODULE_H_
#define _MODULE_H_

#include <dlfcn.h>

#ifndef IS_MODULE
/* App proper header stuff. */
void *load_module(char *path);
void unload_module(void *module);
#else
/* Module header stuff. */
#endif

#endif
