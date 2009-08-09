#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DRAWMAX 7 /* How many numbers get drawn. */
#define SUPLEMNTRY 2 /* Extra, fuckn au. */
#define RESMAX 45 /* How many numbers in the lotto? */
#define ROWS 7    /* How many numbers per colum. */
#define COLUMS 10 /* How many sets of numbers per ticket. */

struct lotto_ticket {
    int numbers[COLUMS][ROWS]; /* How many numbers? maybe multirow. */
    int id;
};

struct lotto_ticket gen_ticket() {
    int i, y;
    struct lotto_ticket ticket;

    memset(&ticket, 0, sizeof(ticket));

    for(i = 0; i < COLUMS; i++)
        for(y = 0; y < ROWS; y++)
            ticket.numbers[i][y] = rand() % RESMAX;
    return ticket;
}

void draw(int *numbers) {
    int i;

    for(i = 0; i < DRAWMAX+SUPLEMNTRY; i++) 
        numbers[i] = rand() % RESMAX;
}

int number_match(int *numbers, int number) {
    int i;

    for(i = 0; i < DRAWMAX; i++)
        if(numbers[i] == number)
            return 1;

    for(i = DRAWMAX; i < DRAWMAX+SUPLEMNTRY; i++)
        if(numbers[i] == number)
            return 2;

    return 0;
}

int main(int argc, char *argv[]) {
    struct lotto_ticket ticket;
    int i, y, numbers[DRAWMAX+SUPLEMNTRY], ncount, scount, nres;

    srand(time(NULL));

    draw(numbers);
    ticket = gen_ticket();

    /* Print numbers. */
    for(i = 0;i < DRAWMAX+SUPLEMNTRY; i++)
        printf("%d\t", numbers[i]);
    printf("\n---------------------------------\n\n");

    /* Print test ticket numbers.*/
    for(i = 0; i < COLUMS; i++) {
        ncount = scount = 0;
        for(y = 0; y < ROWS; y++) {
            nres = number_match(numbers, ticket.numbers[i][y]);
            if(nres == 1) {
                printf("%d*\t", ticket.numbers[i][y]);
                ncount++;
            }
            else if(nres == 2) {
                printf("%d**\t", ticket.numbers[i][y]);
                scount++;
            }
            else
                printf("%d\t", ticket.numbers[i][y]);
        }
        printf("(%d,%d)\n", ncount, scount);
    }
    return 0;
}
