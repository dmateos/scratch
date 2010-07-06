#ifndef _MODULE_H_
#define _MODULE_H_

#include <dlfcn.h>
#include "irc.h"

typedef struct module_data {
    char id[4];
    void *modhandle;

    /* Callbacks. */
    privmsg_cbt privmsg_cb;    

    /* API */
    void(*send_mesg)(connection_t*, char*, char*);
} moddata_t;

#ifndef IS_MODULE
/* App proper header stuff. */
moddata_t *load_module(char *path);
void unload_module(moddata_t *modata);
#endif

#endif
