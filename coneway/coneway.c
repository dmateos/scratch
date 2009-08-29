#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#define X 20
#define Y 20

#define BSIZE 10 /* Squared. */

struct pool {
    char data[Y][X];
};

void oshit(struct pool *pool, char *msg) {
    int x, y;
    printf("OH SHIT: %s\n", msg);

    /* dump cell array. */
    printf("cell dump\n");
    for(y = 0; y < Y; y++) {
        for(x = 0; x < X; x++) {
            printf("%d ", pool->data[y][x]);
        }
        puts("\n");
    }
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
            if(pool->data[y][x] != 0) {
                dest.x = x * BSIZE;
                dest.y = y * BSIZE;
                SDL_FillRect(canvas, &dest, color);
            }
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

            switch(neighb) {
                case 0:
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                    //printf("hits from %dx%d h:%d\n", x, y, neighb);
                    break;
                default:
                    break;
            }
        }
    }
    //printf("---\n");
    return 0;
}

int main(int argc, char **argv) {
    struct pool pool;
    SDL_Surface *display;
    SDL_Event event;

    memset(&pool, 0, sizeof(pool));
    pool.data[0][0] = 1;
    pool.data[9][9] = 1;
    pool.data[7][9] = 1;
    pool.data[19][19] = 1;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        oshit(&pool, "SDL video init.");

    if(!(display = SDL_SetVideoMode(BSIZE*X, BSIZE*Y, 16, SDL_SWSURFACE)))
        oshit(&pool, "SDL Screen mode set");

    while(1) {
        comp_pool(&pool);
        render_pool(display, &pool);

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
    }

    SDL_Quit();
    return 0;
}
