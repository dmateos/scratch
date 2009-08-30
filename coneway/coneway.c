/** 
 * Coneway, Implementation of Conway's_Game_of_Life
 * Daniel Mateos, 2009
 * 
 * To see if i could make a decent game of life over a day and a half and
 * hope its better than PDA's version that i havent seen yet.
 *
 * L to load the sample/test config with cell specs from wikipedia.
 * S to dump current cell pool to disk.
 * C to clear cell pool.
 * O/P to -/+ sim speed by 100ms increments.
 * Mouse button click 1 on square to add new cells and 2 to remove.
 * Enter to start sim.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>

#define X 90
#define Y 90

#define BSIZE 10 /* Squared. */

//#define DEBUG

struct pool {
    char data[X][Y];
    char dirty; /* To keep track of rendering updates. */
};

void oshit(struct pool *pool, char *msg);
int render_pool(SDL_Surface *canvas, struct pool *pool);
int neighbour_count(int cellx, int celly, struct pool *pool);
int comp_pool(struct pool *pool);
void dump_pool(struct pool *pool, char *filename);
void read_pool(struct pool *pool, char *filename);
int main(int argc, char **argv);

void oshit(struct pool *pool, char *msg) {
    int x, y;
    printf("OH SHIT: %s\n", msg);

    x = y = 0; /* GCC warn on -Wall :/ */
#ifdef DEBUG
    /* dump cell array. */
    printf("cell dump\n");
    for(x = 0; x < X; x++) {
        for(y = 0; y < Y; y++)
            printf("%d ", pool->data[x][y]);

        puts("\n");
    }
#endif
    exit(1);
}

int render_pool(SDL_Surface *canvas, struct pool *pool) {
    int x, y, fgc, bgc;
    SDL_Rect dest;

    /* Background and foreground colors for rendering and 
       WxH pixels of each cell on screen. */
    fgc = SDL_MapRGB(canvas->format, 0, 0, 0);
    bgc = SDL_MapRGB(canvas->format, 255, 255, 255);
    dest.w = BSIZE;
    dest.h = BSIZE;

    for(x = 0; x < X; x++) {
        for(y = 0; y < Y; y++) {
            dest.x = x * BSIZE;
            dest.y = y * BSIZE;
            SDL_FillRect(canvas, &dest, (pool->data[x][y] == 1) ? fgc : bgc);
        }
    }
    SDL_Flip(canvas);
    pool->dirty = 0;
    return 0;
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

void dump_pool(struct pool *pool, char *filename) {
    FILE *file = fopen(filename, "w");
    if(fwrite(pool, 1, sizeof(*pool), file) < sizeof(*pool))
        oshit(pool, "Short write on pool dump");
    fclose(file);
}

void read_pool(struct pool *pool, char *filename) {
    FILE *file = fopen(filename, "r");
    if(fread(pool, 1, sizeof(*pool), file) < sizeof(*pool))
        oshit(pool, "Short write on pool read");
    fclose(file);
}

int main(int argc, char **argv) {
    struct pool pool;
    int gencount, go, simspeed, mainloop, mousex, mousey;
    SDL_Surface *display;
    SDL_Event event;
    char wmtbuff[100], mousemask;

    memset(&pool, 0, sizeof(pool));
    gencount = go = 0;
    mainloop = 1;
    simspeed = 500;

    /* Init SDL, display and TTF for font drawing. */
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        oshit(&pool, "SDL video init.");

    if(!(display = SDL_SetVideoMode(BSIZE*X, BSIZE*Y, 16, SDL_SWSURFACE)))
        oshit(&pool, "SDL Screen mode set");

    /* Set title and render initial setup. */
    SDL_WM_SetCaption("CONEways Game of life player, Daniel Mateos", NULL);
    render_pool(display, &pool);

    while(mainloop) {
        /* If go, comp the pool and render it. */
        if(go) {
            comp_pool(&pool);
            pool.dirty = 1;
            gencount++;
            sprintf(wmtbuff, "Generation: %d. Delay: %dms", gencount, simspeed);
            SDL_WM_SetCaption(wmtbuff, NULL);
        }
        /* If not going, check for user clicks for cell placement. */
        else {
            mousemask = SDL_GetMouseState(&mousex, &mousey);
            /* Add cell. */
            if(mousemask & SDL_BUTTON(1)) {
                pool.data[mousex/BSIZE][mousey/BSIZE] = 1;
                pool.dirty = 1;
            }
            if(mousemask & SDL_BUTTON(3)) {
                pool.data[mousex/BSIZE][mousey/BSIZE] = 0;
                pool.dirty =1;
            }
        }

        /* USER IO capture.. */
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    mainloop = 0;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_q:
                            mainloop = 0; 
                            break;
                        /* Pause/Go */
                        case SDLK_RETURN:
                            (go == 0) ? (go = 1) : (go = 0);
                            break;
                        /* Sim speed  */
                        case SDLK_p:
                            (simspeed+100 <= 10000) ? (simspeed += 100) : 0;
                            break;
                        case SDLK_o:
                            (simspeed-100 >= 0) ? (simspeed -= 100) : 0;
                            break;
                        case SDLK_c:
                            memset(&pool, 0, sizeof(pool));
                            pool.dirty = 1;
                            break;
                        case SDLK_s:
                            dump_pool(&pool, "pool.cone");
                            pool.dirty = 1;
                            break;
                        case SDLK_l:
                            read_pool(&pool, "pool.cone");
                            pool.dirty = 1;
                            break;
                        default:
                            break;
                    }
                break;
            }
        }
        /* If dirty, render the pool. */
        if(pool.dirty)
            render_pool(display, &pool);

        SDL_Delay(simspeed);
    }
    SDL_Quit();
    return 0;
}
