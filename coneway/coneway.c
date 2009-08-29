#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#define X 100
#define Y 100

#define BSIZE 10 /* Squared. */

//#define DEBUG

struct pool {
    char data[X][Y];
};

void oshit(struct pool *pool, char *msg) {
    int x, y;
    printf("OH SHIT: %s\n", msg);

#ifdef DEBUG
    /* dump cell array. */
    printf("cell dump\n");
    for(x = 0; x < X; x++) {
        for(y = 0; y < Y; y++) {
            printf("%d ", pool->data[x][y]);
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

    for(x = 0; x < X; x++) {
        for(y = 0; y < Y; y++) {
            dest.x = x * BSIZE;
            dest.y = y * BSIZE;
            if(pool->data[x][y] != 0) {
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
    if((cellx-1 >= 0) && pool->data[cellx-1][celly])
        ncount++;
    /* RIGHT */
    if((cellx+1 < X) && pool->data[cellx+1][celly]) 
        ncount++;
    /* UP */
    if((celly-1 >= 0) && pool->data[cellx][celly-1])
        ncount++;
    /* DOWN */
    if(celly+1 < Y && pool->data[cellx][celly+1])
        ncount++;
    /* UP LEFT */
    if((cellx-1 >= 0) && (celly-1 >= 0) && pool->data[cellx-1][celly-1])
        ncount++;
    /* UP RIGHT. */
    if((cellx+1 < X) && (celly-1 >= 0) && pool->data[cellx+1][celly-1])
        ncount++;
    /* DOWN LEFT */
    if((cellx-1 >= 0) && (celly+1 < Y) && pool->data[cellx-1][celly+1])
        ncount++;
    /* DOWN RIGHT */ 
    if((cellx+1 < X) && (celly+1 < Y) && pool->data[cellx+1][celly+1]) 
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

    for(x = 0; x < X; x++) {
        for(y = 0; y < Y; y++) {
            neighb = neighbour_count(x, y, &snapshot);
            cdata = &pool->data[x][y];

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
    int gencount, go, simspeed;

    memset(&pool, 0, sizeof(pool));
    gencount = go = 0;
    simspeed = 1000;


    /* Toad oscilator. Ocelates for infinity*/
    pool.data[9][7] = 1;
    pool.data[8][7] = 1;
    pool.data[7][7] = 1;
    pool.data[8][8] = 1;
    pool.data[7][8] = 1;
    pool.data[6][8] = 1;

    /* Blinker. Same as aboveish*/
    pool.data[12][12] = 1;
    pool.data[13][12] = 1;
    pool.data[14][12] = 1;

    /* Die hard, dies after 130 its. */
    pool.data[50][70] = 1;
    pool.data[51][70] = 1;
    pool.data[51][71] = 1;
    pool.data[56][69] = 1;
    pool.data[55][71] = 1;
    pool.data[56][71] = 1;
    pool.data[57][71] = 1;
    
    /* Another die hard, syncronicity. */
    pool.data[50][30] = 1;
    pool.data[51][30] = 1;
    pool.data[51][31] = 1;
    pool.data[56][29] = 1;
    pool.data[55][31] = 1;
    pool.data[56][31] = 1;
    pool.data[57][31] = 1;



    /* Init SDL, display and TTF for font drawing. */
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        oshit(&pool, "SDL video init.");

    if(!(display = SDL_SetVideoMode(BSIZE*X, BSIZE*Y, 16, SDL_SWSURFACE)))
        oshit(&pool, "SDL Screen mode set");

    SDL_WM_SetCaption("CONEways Game of life player, Daniel Mateos", NULL);
    while(1) {
        render_pool(display, &pool);

        if(go) {
            comp_pool(&pool);
            gencount++;
            printf("gen: %d\n", gencount);
        }

        /* SDL event capture. */
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    oshit(&pool, "Exited");
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_q:
                            oshit(&pool, "Exited");
                            break;
                        /* Pause/Go */
                        case SDLK_g:
                            (go == 0) ? (go = 1) : (go = 0);
                            break;
                        /* Sim speed  */
                        case SDLK_p:
                            (simspeed+100 <= 10000) ? (simspeed += 100) : 0;
                            break;
                        case SDLK_o:
                            (simspeed-100 >= 0) ? (simspeed -= 100) : 0;
                            break;
                        default:
                            break;
                    }
                break;
            }
        }
        SDL_Delay(simspeed);
    }
    return 0;
}
