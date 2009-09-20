#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/glu.h>

#include "render.h"
#include "level.h"

SDL_Surface *init_gfx(int xres, int yres) {
    SDL_Surface *display;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return NULL;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    if(!(display = SDL_SetVideoMode(xres, yres, 16, SDL_OPENGL)))
        return NULL;

    /* Setup opengl with an orthagonal view (no depth) and a 
       direct mapping of the x/y axis to the screen. (meh) */
    glClearDepth(1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-xres, xres, yres, -yres, -500.0, 500.0);
    //glOrtho(0, 800, 600, 0, -500, 500);    

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(1.0, 1.0, 1.0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    //glRotatef(35.264, 1.0, 0.0, 0.0);
    //glRotatef(-45.0, 0.0, 1.0, 0.0);
    glRotatef(-60, 1, 0, 0);
    glRotatef(-45, 0, 0, 1);

    return display;
}

void gfx_zoom(int xres, int yres, int zoom) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-xres/zoom, xres/zoom, yres/zoom, -yres/zoom, -500, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-60, 1, 0, 0);
    glRotatef(-45, 0, 0, 1);
}

void clear_screen(SDL_Surface *surface) {
    glClear(GL_COLOR_BUFFER_BIT);
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

int draw_level(SDL_Surface *glsurface, struct level *level, int xvp, int yvp) {
    int i;
    rect_t modr;
   
    /* Draw map boarder. */
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);;
    glVertex2f(0+xvp, 0+yvp);
    glVertex2f(level->xmax+xvp, 0+yvp);
    glVertex2f(level->xmax+xvp, level->ymax+yvp);
    glVertex2f(0+xvp, level->ymax+yvp);
    glEnd();

    /* Draw everything else reletive to the viewpoint coords. */
    for(i = 0; i < level->objc; i++) {
        /* We copy to a local rect and modify that so the users world
           coords arnt fucked up permenantly. */
        memcpy(&modr, &level->objs[i]->lrect, sizeof(modr));
        modr.x += xvp;
        modr.y += yvp; 
        draw_rect(glsurface, &modr, 0);
    }
    SDL_GL_SwapBuffers();
    return i;
}
