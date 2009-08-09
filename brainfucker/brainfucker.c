#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void interpreter(char ops[]) {
    int icount, lcnt;
    char memory[1024];
    char *mptr;

    mptr = memory;
    lcnt = 0;

    for(icount = 0; icount < strlen(ops); icount++) {
        switch(ops[icount]) {
            /* IO. */
            case '.':
                putchar(*mptr);
                break;
            case ',':
                *mptr = getchar();
                break;
            /* Incr/deincr value at current memory pointer. */
            case '+':
                *mptr++;
                break;
            case '-':
                *mptr--;
                break;
            /* Incr/deincr memory pointer. */
            case '>':
                mptr++;
                break;
            case '<':
                mptr--;
                break;

            case '[':
                lcnt++;
                break;
            case ']':
                lcnt--;
                break;

            default:
                printf("invalod op\n");
                exit(1);
        }
    }
}

int main(int argc, char *argv[]) {
    char ops[] = ",>,<.>.";
    interpreter(ops);
    return 0;
}
