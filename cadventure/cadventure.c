#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

#include "render.h"
#include "level.h"

void incr_step(struct level *level, struct level_obj *obj) {
    obj->lrect.x += 1;
    obj->lrect.y += 1;
}

void check_bang(struct level *level, struct level_obj *obj) {
    /* Basic on top of detection test. */
    if((obj->lrect.x == level->objs[PLAYER]->lrect.x) &&
       (obj->lrect.y == level->objs[PLAYER]->lrect.y))
            printf("intruder!\n");
}

void load_map(struct level *level) {
    FILE *file; 
    char mbuffer[1024], *bptr;
    int x, y = 0;

    memset(mbuffer, '\0', sizeof(mbuffer));
    file = fopen("map.map", "r");
    fread(mbuffer, 1, 1024, file);
    fclose(file);    

    bptr = strtok(mbuffer, "\n");

    while(bptr != NULL) {
        printf("%s\n", bptr);
        for(x = 0; bptr[x] != '\0'; x++) {
            if(bptr[x] == 'r')
                level_add_obj(level, level_make_obj("wall", NULL, build_rect(x*100, y*100, 100, 100, 1, 0, 0)));
            else if(bptr[x] == 'g')
                level_add_obj(level, level_make_obj("wall", NULL, build_rect(x*100, y*100, 100, 100, 0, 1, 0)));
            else if(bptr[x] == 'b')
                level_add_obj(level, level_make_obj("wall", NULL, build_rect(x*100, y*100, 100, 100, 0, 0, 1)));
        }
        y++;
        bptr = strtok(NULL, "\n");
    }
}

int main(int argc, char *argv[]) {
    Screen *screen;
    struct level *level;
    SDL_Event event;
    int xvp, yvp = 0;

    level = level_init();
    screen = init_gfx();

    level_add_obj(level, level_make_obj("evil!", incr_step, build_rect(70, 0, 10, 10, 0, 1.0, 0)));
    level_add_obj(level, level_make_obj("evil2", check_bang, build_rect(400, 400, 10, 10, 0, 0, 1.0)));
    load_map(level);

    while(1) {
        clear_screen(screen);
        level_comp(level); 
        draw_level(screen, level ,xvp , yvp);

        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    goto clean_exit;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_DOWN:
                            //level->objs[PLAYER]->lrect.y += 10;
                            yvp += 10;
                            break;
                        case SDLK_UP:
                            //level->objs[PLAYER]->lrect.y -= 10;
                            yvp -= 10;
                            break;
                        case SDLK_LEFT:
                            //level->objs[PLAYER]->lrect.x -= 10;
                            xvp -= 10;
                            break;
                        case SDLK_RIGHT:
                            //level->objs[PLAYER]->lrect.x += 10;
                            xvp += 10;
                            break;
                        case SDLK_q:
                            goto clean_exit;
                            break;
                        default:
                            break;
                    }
                    break;
            }
        }

        SDL_Delay(10);
    }

clean_exit:
    level_obj_free(level->objs[1]);
    level_obj_free(level->objs[2]);
    level_free(level);
    return 0;
}

