#include <stdio.h>
#include <stdlib.h>

#include "chunkify.h"

int main(int argc, char **argv) {
    chunk_t *data;
    int chunkc;

    if(argc ==2 )
        data = chunkify(argv[1], &chunkc);
    else 
        data =chunkify("/sbin/ifconfig", &chunkc);    

    free(data);
}
