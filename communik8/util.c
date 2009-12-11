#include <stdlib.h>
#include <stdio.h>
#include "util.h"

void *emalloc(int size) {
    void *ptr = malloc(size+sizeof(int));
    if(ptr == NULL) {
        printf("memory alloc error\n");
        exit(1);
    }
#ifdef _MEMDEBUG
    ((int*)ptr)[0] = size;
    printf("dynamic alloc of %d\n", size);
    return ((int*)ptr)+1;
#else
    return ptr;
#endif
}

void efree(void *data) {
    if(data == NULL)
        return;

#ifdef _MEMDEBUG
    printf("freeing ptr of size %d\n", *(((int*)data)-1));
    free(((int*)data)-1);
#else
    free(data);
#endif
}

