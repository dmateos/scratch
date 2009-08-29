#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#define X 50
#define Y 50

#define BSIZE 10 /* Squared. */

#define DEBUG

struct pool {
    char data[Y][X];
};

void oshit(struct pool *pool, char *msg) {
    int x, y;
    printf("OH SHIT: %s\n", msg);

#ifdef DEBUG
    /* dump cell array. */
    printf("cell dump\n");
    for(y = 0; y < Y; y++) {
        for(x = 0; x < X; x++) {
            printf("%d ", pool->data[y][x]);
        }
        puts("\n");
    }
#endif
    exit(1);
}

int render_pool(SDL_Surface *canvas, struct pool *pool) {
    int x, y, color;
    SDL_Rect dest;

    color = SDL_MapRGB(canvas->format, 0, 255, 0);
    dest.w = BSIZE;
    dest.h = BSIZE;

    for(y = 0; y < Y; y++) {
        for(x = 0; x < X; x++) {
            dest.x = x * BSIZE;
            dest.y = y * BSIZE;
            if(pool->data[y][x] != 0) {
                SDL_FillRect(canvas, &dest, color);
            }
            else
                SDL_FillRect(canvas, &dest, 0);
        }
    }
    SDL_Flip(canvas);
}

int neighbour_count(int cellx, int celly, struct pool *pool) {
    int ncount = 0;

    /* LEFT */
    if((cellx-1 >= 0) && pool->data[celly][cellx-1])
        ncount++;
    /* RIGHT */
    if((cellx+1 < X) && pool->data[celly][cellx+1]) 
        ncount++;
    /* UP */
    if((celly-1 >= 0) && pool->data[celly-1][cellx])
        ncount++;
    /* DOWN */
    if(celly+1 < Y && pool->data[celly+1][cellx])
        ncount++;
    /* UP LEFT */
    if((cellx-1 >= 0) && (celly-1 >= 0) && pool->data[celly-1][cellx-1])
        ncount++;
    /* UP RIGHT. */
    if((cellx+1 < X) && (celly-1 >= 0) && pool->data[celly-1][cellx+1])
        ncount++;
    /* DOWN LEFT */
    if((cellx-1 >= 0) && (celly+1 < Y) && pool->data[celly+1][cellx-1])
        ncount++;
    /* DOWN RIGHT */ 
    if((cellx+1 < X) && (celly+1 < Y) && pool->data[celly+1][cellx+1]) 
        ncount++;

    return ncount;
}

int comp_pool(struct pool *pool) {
    int x, y, neighb;
    struct pool snapshot;
    char *cdata;

    /* snapshot the pool to apply transformation from to sim all cells 
       reacting at once. */
    memcpy(&snapshot, pool, sizeof(snapshot));

    for(y = 0; y < Y; y++) {
        for(x = 0; x < X; x++) {
            neighb = neighbour_count(x, y, &snapshot);
            cdata = &pool->data[y][x];

#ifdef DEBUG
            printf("hits from %dx%d h:%d\n", x, y, neighb);
#endif
            switch(neighb) {
                /* Rule 1: Any live cell with fewer than two live neighbours 
                   dies, as if caused by underpopulation. */
                case 0:
                case 1:
                    if(*cdata == 1)
                        *cdata = 0;
                    break;
                /* Rule 3: Any live cell with two or three live neighbours 
                 * lives on to the next generation. */
                case 2:
                    break;
                /* Rule 4: Any dead cell with exactly three live neighbours 
                   becomes a live cell. */
                case 3:
                    if(*cdata == 0)
                        *cdata = 1;
                    break;
                /* Rule 2: Any live cell with more than three live neighbours 
                   dies, as if by overcrowding. */
                default:
                    if(*cdata == 1)
                        *cdata = 0;
                    break;
            }
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    struct pool pool;
    SDL_Surface *display;
    SDL_Event event;
    unsigned long gencount;

    memset(&pool, 0, sizeof(pool));
    gencount = 0;

    /* Toad oscilator. Ocelates for infinity*/
    pool.data[7][9] = 1;
    pool.data[7][8] = 1;
    pool.data[7][7] = 1;
    pool.data[8][8] = 1;
    pool.data[8][7] = 1;
    pool.data[8][6] = 1;

    /* Blinker. Same as aboveish*/
    pool.data[12][12] = 1;
    pool.data[12][13] = 1;
    pool.data[12][14] = 1;

    /* The infamous F-pentomino, dies after 130 gens appar */
    pool.data[32][24] = 1;
    pool.data[32][23] = 1;
    pool.data[33][23] = 1;
    pool.data[33][22] = 1;
    pool.data[34][23] = 1;

    /* Init SDL, display and TTF for font drawing. */
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        oshit(&pool, "SDL video init.");

    if(!(display = SDL_SetVideoMode(BSIZE*X, BSIZE*Y, 16, SDL_SWSURFACE)))
        oshit(&pool, "SDL Screen mode set");

    SDL_WM_SetCaption("CONEways Game of life player, Daniel Mateos", NULL);
    while(1) {
        comp_pool(&pool);
        render_pool(display, &pool);
        gencount++;

        /* SDL event capture. */
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    oshit(&pool, "Exited");
                    break;
                default:
                    break;
            }
        }
        SDL_Delay(1000);
    }
    SDL_Quit();
    return 0;
}
