#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/glu.h>

#include "render.h"

SDL_Surface *init_gfx() {
    SDL_Surface *display;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return NULL;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    if(!(display = SDL_SetVideoMode(800, 600, 16, SDL_OPENGL)))
        return NULL;

    /* Setup opengl with an orthagonal view (no depth) and a 
       direct mapping of the x/y axis to the screen. (meh) */
    glClearDepth(1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0.0, -1.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, 800, 600);
    glClearColor(1.0, 1.0, 1.0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    return display;
}

void clear_screen(SDL_Surface *surface) {
    glClear(GL_COLOR_BUFFER_BIT);
}

inline rect_t build_rect(int x, int y, int w, int h, double r, double g, double b) {
    rect_t rect;

    rect.x = x; rect.y = y;
    rect.w = w; rect.h = h;
    rect.r = r; rect.g = g; rect.b = b;

    return rect;
}

inline int draw_rect(SDL_Surface *glsurface, rect_t *rect, int flush) {
    /* Set color and draw rect as a GL quad. */
    glColor3f(rect->r, rect->g, rect->b);

    glBegin(GL_QUADS);
    glVertex2f(rect->x, rect->y);
    glVertex2f(rect->x+rect->w, rect->y);
    glVertex2f(rect->x+rect->w, rect->y+rect->h);
    glVertex2f(rect->x, rect->y+rect->h);
    glEnd();

    if(flush)
        SDL_GL_SwapBuffers();
    return 0;
}

int draw_rects(SDL_Surface *glsurface, int flush, ...) {
    va_list ap;
    int rcount;
    rect_t *rectp;

    rcount = 0;
    va_start(ap, flush);

    /* Run draw_rect on each rect arg untill NULL. */
    while((rectp = va_arg(ap, rect_t*)) != NULL) {
        draw_rect(glsurface, rectp, 0);
        rcount++;
    }

    va_end(ap);
    if(flush)
        SDL_GL_SwapBuffers();
    return rcount;
}

