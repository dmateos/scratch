#include <stdio.h>
#include <stdlib.h>
#include <crypt.h>

#define BSIZE 95
#define BQUAD ((((BSIZE * BSIZE) * BSIZE) * BSIZE) * 5)

int precomp_array(char *buffer) {
    int i,j,k,l;
    long tot = 0;
 
    for(i = 32; i < 127; i++) {
        for(j = 32; j < 127; j++) {
            for(k = 32; k < 127; k++) {
                for(l = 32; l < 127; l++) {
                    if(tot >= BQUAD) {
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
    char *buffer = malloc(BQUAD);
    long i = 0;

    if(buffer == NULL)
        return 1;

    printf("malloced %ld byte buffer\n", ((long)BQUAD));
    precomp_array(buffer);

    while(1) {
        //printf("comparing %s->%s to %s\n", buffer+i, crypt(buffer+i, "je"), argv[1]);
        if(strcmp(crypt(buffer+i, "je"), argv[1]) == 0) {
            printf("%s is %s\n", argv[1], buffer+i);
            break;
         }
        if(!((i/5) % 10000000))
            printf("UPTO %ld %s\n", i/5, buffer+i);
        i+=5;
    }
    free(buffer);
    return 0;
}
