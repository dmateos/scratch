 #include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int i, max;

    if(argc <= 1)
        exit(1);

    max = atoi(argv[1]);

    for(i = 0; i <= max; i++)
        if((i % 3 == 0) && (i % 5 == 0))
            printf("%d FuzzBuzz\n", i);
        else if(i % 3 == 0)
            printf("%d Fuzz\n", i);
        else if(i % 5 == 0)
            printf("%d Buzz\n", i);

    return 0;
}
