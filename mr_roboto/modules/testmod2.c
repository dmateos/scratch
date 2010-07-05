#define IS_MODULE

#include "../module.h"
#include <stdio.h>

void mod_main() {
    printf("testmod2: i am another mod\n");
}

void mod_exit() {
    printf("testmod2: my cup also runneth over\n");
}
