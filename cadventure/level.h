#ifndef _LEVEL_H
#define _LEVEL_H

/* Object location for the special player object. */
#define PLAYER 0

/* Rectangle for representing objects in a 2d space. */
typedef struct render_rect {
    int w,h,x,y;
    float r,g,b;
} rect_t;

/* Macro to ease the definition of default action callbacks. */
#define DEFACTION(x) void(*x)(struct level*, struct level_obj*)

/* Represents a game level and contains all its required data. */
struct level {
    int xmax, ymax, objc;
    struct level_obj *objs[2048];
};

/* Represents an object that exsists within a level. */
struct level_obj {
    char id[16];
    rect_t lrect;
    DEFACTION(default_action);
};

rect_t build_rect(int x, int y, int w, int h, double r, double g, double b);
struct level *level_init(int xmax, int ymax);
void level_free(struct level *level);
int level_add_obj(struct level *level, struct level_obj *object);
int level_comp(struct level *level);
struct level_obj *level_make_obj(char *id, DEFACTION(default_action), rect_t rect);
void level_obj_free(struct level_obj *object);
#endif
