/** 
 * Coneway, Implementation of Conway's_Game_of_Life
 * Daniel Mateos, Sep 2009
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
#include <SDL/SDL.h>

#define X 90
#define Y 90
#define BSIZE 10 /* Squared. */
#define FGCOLOR 0, 0, 255
#define BGCOLOR 255, 255, 255
//#define DEBUG

/* Note: cooler way to do this when i have time:
   keep track of dirty for each element by turning
   the 0/1 char switch into a bitfield.  So never fear
   the black magic defines bellow are currently unused.
#define CHECK_LIVE(p)   (p & (1 << 0))
#define SET_LIVE(p)     (p |= (1 << 0))
#define UNSET_LIVE(p)   (p &= ~(1 << 0))
#define CHECK_DIRTY(p)  (p & ( 1 << 1))
#define SET_DIRTY(p)    (p |= (1 << 1))
#define UNSET_DIRTY(p)  (p &= ~(1 << 1)) */
struct pool {
    char data[X][Y];
    char dirty; /* To keep track of rendering updates. */
    int gencount;
};

void oshit(const struct pool *pool, char *msg, int fatal);
int draw_pool(struct pool *pool, SDL_Surface *canvas);
int neighb_pool(const struct pool *pool, int cellx, int celly);
int comp_pool(struct pool *pool);
void save_pool(const struct pool *pool, char *filename);
void open_pool(struct pool *pool, char *filename);
int main(int argc, char **argv);

void oshit(const struct pool *pool, char *msg, int fatal) {
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
    if(fatal)
        exit(1);
}

int draw_pool(struct pool *pool, SDL_Surface *canvas) {
    int x, y, fgc, bgc;
    SDL_Rect dest;

    /* Background and foreground colors for rendering and 
       WxH pixels of each cell on screen. */
    fgc = SDL_MapRGB(canvas->format, FGCOLOR);
    bgc = SDL_MapRGB(canvas->format, BGCOLOR);
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

int neighb_pool(const struct pool *pool, int cellx, int celly) {
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
    if((celly+1 < Y) && pool->data[cellx][celly+1])
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
    int x, y, neighbcount;
    struct pool poolstate;
    char *cdata;

    /* snapshot the pool state to apply transformation from to sim all cells 
       reacting at once instead of to the modified active cells. */
    memcpy(&poolstate, pool, sizeof(poolstate));

    for(x = 0; x < X; x++) {
        for(y = 0; y < Y; y++) {
            neighbcount = neighb_pool(&poolstate, x, y);
            cdata = &pool->data[x][y];

#ifdef DEBUG
            printf("hits from %dx%d h:%d\n", x, y, neighb);
#endif
            switch(neighbcount) {
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

void save_pool(const struct pool *pool, char *filename) {
    FILE *file = fopen(filename, "w");
    if(file != NULL) {
        if(fwrite(pool, 1, sizeof(*pool), file) < sizeof(*pool))
            oshit(pool, "Short write on pool dump, not likley", 0);
        fclose(file);
    }
}

void open_pool(struct pool *pool, char *filename) {
    FILE *file = fopen(filename, "r");
    if(file != NULL) {
        if(fread(pool, 1, sizeof(*pool), file) < sizeof(*pool)) {
            oshit(pool, "Short read on load, maybe pool size/ver diff", 0);
            /* Recover by just giving zeroed data or garbage could be shown. */
            memset(pool, 0, sizeof(*pool));
        }
        fclose(file);
    }
}

int main(int argc, char **argv) {
    int go, simspeed, mainloop, mousex, mousey;
    struct pool pool;
    char wmtbuff[150], mousemask;
    SDL_Surface *display;
    SDL_Event event;

    go = 0;
    mainloop = 1;
    simspeed = 500;
    memset(&pool, 0, sizeof(pool));

    /* Init SDL, display and TTF for font drawing. */
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        oshit(&pool, "SDL video init.", 1);

    if(!(display = SDL_SetVideoMode(BSIZE*X, BSIZE*Y, 16, SDL_SWSURFACE)))
        oshit(&pool, "SDL Screen mode set", 1);

    /* Set title and render initial setup. */
    draw_pool(&pool, display);

    while(mainloop) {
        /* If go, comp the pool and render it. */
        if(go) {
            comp_pool(&pool);
            pool.dirty = 1;
            pool.gencount++;
        }
        /* If not going, check for user clicks for cell placement. */
        /* USER IO capture.. */
        else {
            mousemask = SDL_GetMouseState(&mousex, &mousey);
            /* Add cell. */
            if(mousemask & SDL_BUTTON(1)) {
                pool.data[mousex/BSIZE][mousey/BSIZE] = 1;
                pool.dirty = 1;
            }
            /* BELETED! */
            else if(mousemask & SDL_BUTTON(3)) {
                pool.data[mousex/BSIZE][mousey/BSIZE] = 0;
                pool.dirty = 1;
            }
        }

        /* More user io capture. */
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
                            (simspeed+50 <= 2000) ? (simspeed += 50) : 0;
                            break;
                        case SDLK_o:
                            /* Had min speed as 0, runs fast but fucks up X 
                               with title updates, NOTE: set back when font
                               engine written. */
                            (simspeed-50 >= 50) ? (simspeed -= 50) : 0;
                            break;
                        case SDLK_c:
                            memset(&pool, 0, sizeof(pool));
                            pool.dirty = 1;
                            pool.gencount = 0;
                            break;
                        case SDLK_s:
                            save_pool(&pool, "pool.cone");
                            pool.dirty = 1;
                            break;
                        case SDLK_l:
                            open_pool(&pool, "pool.cone");
                            pool.dirty = 1;
                            pool.gencount = 0;
                            break;
                        default:
                            break;
                    }
                break;
            }
        }
        /* If dirty, render the pool. */
        if(pool.dirty)
            draw_pool(&pool, display);
        
        /* Update title bar. */
        snprintf(wmtbuff, sizeof(wmtbuff),
                "Coneway - %dx%d - Gen: %d, Delay: %dms, Running: %s",
                 X, Y, pool.gencount, simspeed, go ? "True" : "False");
        SDL_WM_SetCaption(wmtbuff, NULL);

        /* If going we delay by user spec if not 10ms so editing isnt laggy. */
        (go) ? SDL_Delay(simspeed) : SDL_Delay(10);
    }
    SDL_Quit();
    return 0;
}
