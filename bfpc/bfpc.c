#include <stdio.h>
#include <stdlib.h>
#include <crypt.h>
#include <string.h>

#define CRANGE 95
#define WSIZE 5
#define CSIZE ((((CRANGE * CRANGE) * CRANGE) * CRANGE) * WSIZE)

int precomp_array(char *buffer) {
    int i,j,k,l;
    long tot = 0;
 
    for(i = 32; i < 127; i++) {
        for(j = 32; j < 127; j++) {
            for(k = 32; k < 127; k++) {
                for(l = 32; l < 127; l++) {
                    if(tot >= CSIZE) {
                        printf("out of space at %ld %d %d %d %d", tot, i,j,k,l);
                        return -1;
                    }
                    *(buffer+tot++) = i;
                    *(buffer+tot++) = j;
                    *(buffer+tot++) = k;
                    *(buffer+tot++) = l;
                    *(buffer+tot++) = '\0';
                }
            }
        }
    }
    printf("build db of %ld bytes and %ld entries\n", tot, tot/5);
    return 0;
}

int main(int argc, char **argv) {
    char *buffer = malloc(CSIZE);
    char seed[3];
    long i = 0;

    if(buffer == NULL)
        return 1;

    if(argc < 1) {
        printf("wrong args\n");
        return -1;
    }

    /* Grab seed from start of hash. */
    memcpy(seed, argv[1], 2);
    seed[3] = '\0'; 

    printf("searching for key for %s with seed %s\n", argv[1], seed);
    printf("malloced %ld byte search buffer\n", ((long)CSIZE));
    precomp_array(buffer);

    while(1) {
        //printf("comparing %s->%s to %s\n", buffer+i, crypt(buffer+i, "je"), argv[1]);
        if(strcmp(crypt(buffer+i, seed), argv[1]) == 0) {
            printf("found!, %s is %s at loc %ld", argv[1], buffer+i, i/WSIZE);
            break;
         }
        if(!((i/WSIZE) % 10000000))
            printf("not yet, upto %ld %s\n", i/WSIZE, buffer+i);
        i+=5;
    }
    free(buffer);
    return 0;
}
