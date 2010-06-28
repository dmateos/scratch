#define IS_MODULE

#include "../module.h"
#include <stdio.h>

void mod_main() {
    printf("testmod: i am a mod\n");
}

void mod_exit() {
    printf("testmod: my cup runneth over\n");
}
