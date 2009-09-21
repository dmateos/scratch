#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "level.h"
#include "render.h"

inline rect_t build_rect(int x, int y, int w, int h, double r, double g, double b) {
    rect_t rect;

    rect.x = x; rect.y = y;
    rect.w = w; rect.h = h;
    rect.r = r; rect.g = g; rect.b = b;

    rect.texture = 0;
    return rect;
}

struct level *level_init(int xsize, int ysize) {
    struct level *level;

    /* Allocate level and initial player object, 
       mind the nested function calls. */
    level = malloc(sizeof *level);
    memset(level, 0, sizeof *level);
    level->xmax = xsize;
    level->ymax = ysize;
    level_add_obj(level, level_make_obj("player", NULL, build_rect(10, 10, 10, 10, 0, 0, 1)));

    return level;
}

void level_free(struct level *level) {
    free(level->objs[PLAYER]);
    free(level);
}

int level_add_obj(struct level *level, struct level_obj *object) {
    level->objs[level->objc++] = object;
    printf("obj:%s @ %dx%d, slot %d, daction: %p\n", 
            object->id, object->lrect.x, 
            object->lrect.y, level->objc, 
            object->default_action);
    return 0;
}

int level_comp(struct level *level) {
    int i;
    struct level_obj *lptr;

    for(i = 0; i < level->objc; i++) {
        lptr = level->objs[i];
        //printf("Computing object %s\n", lptr->id);
        if(lptr->default_action != NULL) {
            lptr->default_action(level, lptr);
            //printf("performed callback at %p\n", lptr->default_action);
        }
    }
    return i;
}

struct level_obj *level_make_obj(char *id, DEFACTION(default_action), rect_t rect) {
    struct level_obj *obj;

    /* Alocate level obj and prep its attributes. */
    obj = malloc(sizeof *obj);
    memset(obj, 0, sizeof *obj);
    memcpy(obj->id, id, strlen(id));
    obj->lrect = rect;
    obj->default_action = default_action;
    return obj;
}

void level_obj_free(struct level_obj *object) {
    free(object);
}
