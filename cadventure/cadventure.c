#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

#include "render.h"
#include "level.h"

void incr_step(struct level *level, struct level_obj *obj) {
    if(obj->lrect.y < 300)
        obj->lrect.y += 1;
    else
        obj->lrect.y = 10;
}

void check_bang(struct level *level, struct level_obj *obj) {
    /* Basic on top of detection test. */
    if((obj->lrect.x == level->objs[PLAYER]->lrect.x) &&
       (obj->lrect.y == level->objs[PLAYER]->lrect.y))
            printf("intruder!\n");
}

int load_map(struct level *level, char *mname) {
    FILE *file; 
    char mbuffer[1024], *bptr;
    int x, y = 0;

    /* Read the file into buffer. */
    memset(mbuffer, '\0', sizeof(mbuffer));
    if((file = fopen(mname, "r")) == NULL)
        return -1;
    fread(mbuffer, sizeof(char), 1024, file);
    fclose(file);    

    /* Split on \n and go thru each line. */
    bptr = strtok(mbuffer, "\n");
    while(bptr != NULL) {
        printf("%s\n", bptr);
        /* For each element in each line. */
        for(x = 0; bptr[x] != '\0'; x++) {
            switch(bptr[x]) {
                /* For now red,green.blue wall and 'evil' walker. */
                case 'r':
                    level_add_obj(level, level_make_obj("wall", NULL, build_rect(x*10, y*10, 10, 10, 1, 0, 0)));
                    break;
                case 'g':
                    level_add_obj(level, level_make_obj("wall", NULL, build_rect(x*10, y*10, 10, 10, 0, 1, 0)));
                    break;
                case 'b':
                    level_add_obj(level, level_make_obj("wall", NULL, build_rect(x*10, y*10, 10, 10, 0, 0, 1)));
                    break;
                case 'e':
                    level_add_obj(level, level_make_obj("evil!", incr_step, build_rect(x*10, y*10, 10, 10, 0, 1.0, 0)));
                    break;
                default:
                    break;
            }
        }
        bptr = strtok(NULL, "\n");
        y++;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    Screen *screen;
    struct level *level;
    SDL_Event event;
    int xvp, yvp, zlevel = 0;
    int mousemask, mousex, mousey;

    level = level_init(300, 300);
    load_map(level, "map.map");
    screen = init_gfx(1024, 768);

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
                    mousemask = SDL_GetMouseState(&mousex, &mousey);
                    if(mousemask & SDL_BUTTON(1)) {
                        xvp = -mousex/2;
                        yvp = -mousey/2;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_DOWN:
                            //level->objs[PLAYER]->lrect.y += 10;
                            yvp -= 10;
                            break;
                        case SDLK_UP:
                            //level->objs[PLAYER]->lrect.y -= 10;
                            yvp += 10;
                            break;
                        case SDLK_LEFT:
                            //level->objs[PLAYER]->lrect.x -= 10;
                            xvp += 10;
                            break;
                        case SDLK_RIGHT:
                            //level->objs[PLAYER]->lrect.x += 10;
                            xvp -= 10;
                            break;
                        case SDLK_q:
                            goto clean_exit;
                            break;
                        case SDLK_p:
                            gfx_zoom(1024, 768, ++zlevel);
                            break;
                        case SDLK_o:
                            if(zlevel != 0)
                                gfx_zoom(1024, 768, --zlevel);
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

