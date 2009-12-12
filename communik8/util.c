#include <stdlib.h>
#include <stdio.h>
#include "util.h"

void *emalloc(int size) {
    /* Catch any malloc errors. */
    void *ptr = malloc(size+sizeof(int));
    if(ptr == NULL) {
        printf("memory alloc error\n");
        exit(1);
    }
/* Pretty hacksie but this stores the size of the var at the
 * beginning of the block and returns it +1 so it can be debugged 
 * with efree. */
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

/* Equiv of above, read the size then free the pointer 
 * put back to the place free() expects. */
#ifdef _MEMDEBUG
    printf("freeing ptr of size %d\n", *(((int*)data)-1));
    free(((int*)data)-1);
#else
    free(data);
#endif
}

