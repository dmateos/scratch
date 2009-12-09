/*
 * Practicing genetic algoritms
 * Daniel Mateos
 * Problem: find an equation out of 0-9 *+/- that comes up with 42
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define GENES 10
#define CHROMOSOMES 16
#define TARGETVAL 10 //42
#define CROSSOVER 70

struct chromosome {
    char *gene[GENES]; /* Genes. */
    float fitness;
    int sum; /* Sum of the genes. */
};

/* Got prototypes bitch? */
static int binstr2dec(char *binstr);
static void init_chromosomes(struct chromosome c[], int count);
static void kill_chromosomes(struct chromosome c[], int count);
static void print_chromosomes(struct chromosome c[], int count);
static int parse_chromosome(struct chromosome *c, int *resbuff, int bsize);
static int set_fitness(struct chromosome *c);
static struct chromosome *select_chromosome(struct chromosome c[], int count);
static void crossover_chromosomes(struct chromosome *c1, struct chromosome *c2);
static void mutate_chromosome(struct chromosome *c);
int main(int argc, char *argv[]);

/* Convert a string of chars representing binary into a decimal value. */
static int binstr2dec(char *binstr) {
    int i, val, valadd;
    val = 0;
    valadd = 1;
    for(i = strlen(binstr); i > 0; i--) {
        if(binstr[i-1] == '1')
            val += valadd;        
        valadd *= 2;
    }
    return val;
}

/* Init an array of chromosome structures with random values. */
static void init_chromosomes(struct chromosome c[], int count) {
    int i, y, z;

    srand(time(NULL));
    /* For each chromosone. */
    for(i = 0; i < count; i++) {
        c[i].fitness = 0;
        /* For each gene. */
        for(y = 0; y < GENES; y++) {
            c[i].gene[y] = malloc(5); /* Each gene stores 4 bins + a null. */
            /* Each gene char. */
            for(z = 0; z < 5; z++) {
                c[i].gene[y][z] = (rand() % 2) ? '1' : '0';
            }
            c[i].gene[y][4] = '\0';
        }
    }
}

/* Free up memory allocation of chromosomes, hi valgrind!. */
static void kill_chromosomes(struct chromosome c[], int count) {
    int i, y;
    for(i = 0; i < count; i++)
        for(y = 0; y < GENES; y++)
            free(c[i].gene[y]);
}

/* Crudely print the chromosome structure and a basic translation of
 * its insides. */
static void print_chromosomes(struct chromosome c[], int count) {
    int i, y, parsebuff[1024], pcnt;

    for(i = 0; i < count; i++) {
        printf("CHROMOSOME %d/%d fitness: %f\n", i+1, count, c[i].fitness);
        pcnt = parse_chromosome(&c[i], parsebuff, sizeof(parsebuff)); 

        /* Dump the raw geognome. */
        for(y = 0; y < GENES; y++)
            printf("%s(%d) ", c[i].gene[y], binstr2dec(c[i].gene[y]));
        printf("\n");

        /* Dump the parsed gengonome */
        for(y = 0; y < pcnt; y++)
            switch(parsebuff[y]) {
                case 10:
                    printf("+ ");
                    break;
                case 11:
                    printf("- ");
                    break;
                case 12:
                    printf("* ");
                    break;
                case 13:
                    printf("/ ");
                    break;
                default:
                    printf("%d ", parsebuff[y]);
                    break;
            }
        printf("\n");
        /* Dump the sum. */
        printf("%d/%d\n", c[i].sum, TARGETVAL);
    }
}

/* Parse a chromosomes gene buffer into number -> operator pairs,
 * note that we disgard anything between that doesnt make sense. */
static int parse_chromosome(struct chromosome *c, int *resbuff, int bsize) {
    int operator, count;
    int i, genedec, *origb;

    count = operator = 0;
    origb = resbuff;
    operator = 1;
    memset(resbuff, '\0', bsize);

    for(i = 0; i < GENES; i++) {
        genedec = binstr2dec(c->gene[i]);

        /* Looking for an operator? */
        if(operator) {
            /* Is operator. */
            if(genedec > 9 && genedec < 14) {
                *resbuff = genedec;
                resbuff++;
                count++;
                operator = 0;
            }
        }
        else {
            /* Number. */
            if(genedec < 9) {
                *resbuff = genedec;
                resbuff++;
                count++;
                operator = 1;
            }
        }
    }

    /* Yeah if in our sum theres a / 0 anywhere bad shit
     * will happen so lets just make it + 0. */
    for(i = 0; i < count; i++) {
        if(origb[i] == 13 && origb[i+1] == 0)
            origb[i] = 10;
    }
    return count;
}

/* Sets a fitness level based on how close the chromosomes
 * genes are to matching the number wanted. */
static int set_fitness(struct chromosome *c) {
    int i, sum, resbuff[1024], rcount;
    float fitness;
    rcount = parse_chromosome(c, resbuff, sizeof(int) * 1024);
    sum = 0;
    fitness = 0.0;

    /* Catch +*-/ and operate the next value upon them. */
    for(i = 0; i < rcount; i+=2) {
        switch(resbuff[i]) {
            case 10:
                sum += resbuff[i+1];
                break;
            case 11:
                sum -= resbuff[i+1];
                break;
            case 12:
                sum *= resbuff[i+1];
                break;
            case 13:
                sum /= resbuff[i+1];
                break;
        }
    }

    /* Fuck yeah target value. */
    if(sum == TARGETVAL)
        fitness = 999.0;
    else
        fitness = 1/fabs((TARGETVAL - sum));

    c->fitness = fitness;
    c->sum = sum;
    return sum;
}

static struct chromosome *select_chromosome(struct chromosome c[], int count) {
    return &c[(rand() % count)]; /* Haw haw */
}

static void crossover_chromosomes(struct chromosome *c1, struct chromosome *c2) {
    int chance, crosspnt; 

    chance = rand() % 100;
    if(chance < CROSSOVER) {
        crosspnt = rand() % GENES;
    }
}

static void mutate_chromosome(struct chromosome *c) {

}

int main(int argc, char *argv[]) {
    int i, resbuff[1024], cont;
    struct chromosome chromes[CHROMOSOMES];
    memset(chromes, '\0', sizeof(chromes));   
 
    cont = 1;

    /* Init the chromosome structure and set initial fitness levels. */
    init_chromosomes(chromes, CHROMOSOMES); 
    for(i = 0; i < CHROMOSOMES; i++)
        set_fitness(&chromes[i]);
    print_chromosomes(chromes, CHROMOSOMES);

    /* Do stuff. */   
    while(cont) {
        crossover_chromosomes(select_chromosome(chromes, CHROMOSOMES), select_chromosome(chromes, CHROMOSOMES));
        for(i = 0; i < CHROMOSOMES; i++) {
            if(set_fitness(&chromes[i]) == 999.0)
                cont = 0;
        }
        print_chromosomes(chromes, CHROMOSOMES);
    }
 
    print_chromosomes(chromes, CHROMOSOMES);
    kill_chromosomes(chromes, CHROMOSOMES);
    return 0;
}
