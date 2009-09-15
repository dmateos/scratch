/* Basic engine for rendering 2d objects, tries to abstract the underlying
   lib deps as much as possible. */
#ifndef _RENDER_H
#define _RENDER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/glu.h>

/* Renderable rectangular object. */
typedef struct render_rect {
    int w,h,x,y;
    float r,g,b;
} rect_t;

/* Abstract our dependence on SDL. */
typedef SDL_Surface Screen;

/* Init the gfx subsystem. */
SDL_Surface *init_gfx();

/* Clear the surface. */
void clear_screen(SDL_Surface *surface);

/* Build rectangle object. */
inline rect_t build_rect(int x, int y, int w, int h, double r, double g, double b);

/* Draw rectangle object to surface. */
inline int draw_rect(SDL_Surface *glsurface, rect_t *rect, int flush); 

/* Draw a bunnch of rectangle objects to surface, MUST end with NULL terminator. */
int draw_rects(SDL_Surface *glsurface, int flush, ...);

#endif
