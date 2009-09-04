#include <stdio.h>
#include <crypt.h>

int main(int argc, char **argv) {
    if(argc < 2) {
        printf("2 args\n");
        return -1;
    }

    printf("%s\n", crypt(argv[1], argv[2]));
    return 0;
}
