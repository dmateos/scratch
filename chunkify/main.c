#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "chunkify.h"

int main(int argc, char **argv) {
    chunk_t *data;
    int chunkc, opt, isserver, isclient;
    char filepath[1024];

    memset(filepath, '\0', sizeof filepath);
    isserver = 0;
    isclient = 0;

    /* Parse the cmd args. */
    while((opt = getopt(argc, argv, "f:sc:")) != -1) {
        switch(opt) {
            /* File to load or save to. */
            case 'f':
                strncpy(filepath, optarg, sizeof filepath);
                break;
            /* Listen. */
            case 's':
                isserver = 1;
                break;
            /* Client. */
            case 'c':
                isclient = 1;
                break;
            default:
                break;
        }
    }


    /* Chunkasize if were the server and path is set. */
    if(isserver && (filepath[0] != '\0')) {
        printf("%s\n", filepath);
        data = chunkify(filepath, &chunkc);
        free(data);
    }
    /* Were a client. */
    else if(isclient && (filepath[0] != '\0')) {

    }
    else {
        fprintf(stderr, "you gave some fucked up params\n");
        return 1;
    }

    return 0;
}
