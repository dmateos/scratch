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
    glOrtho(-xres, xres, yres, -yres, -0.1, 2.0);
    //gluPerspective(45.0f, (GLfloat)xres/(GLfloat)yres, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(1.0, 1.0, 1.0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glRotatef(-35, 1.0, 0.0, 0.0);
    glRotatef(-45.0, 0.0, 0.0, 1.0);
    //glEnable(GL_TEXTURE_2D);
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glViewport(0, 0, 1024, 768);
    return display;
}

void gfx_zoom(int xres, int yres, int zoom) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0/zoom, xres/zoom, yres/zoom, 0/zoom, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(35.264, 1, 0, 0);
    glRotatef(-45, 0, 1, 0);
}

void clear_screen(SDL_Surface *surface) {
    glClear(GL_COLOR_BUFFER_BIT);
}

inline int draw_rect(SDL_Surface *glsurface, rect_t *rect, int flush) {
    /* Set color and draw rect as a GL quad. */
    glColor3f(rect->r, rect->g, rect->b);

    //if(rect->texture > 0)
        glBindTexture(GL_TEXTURE_2D, rect->texture);

    glBegin(GL_QUADS);
    glTexCoord3f(0.0, 0.0, 1.0);
    glVertex2f(rect->x, rect->y);

    glTexCoord3f(1.0, 0.0, 1.0);
    glVertex2f(rect->x+rect->w, rect->y);

    glTexCoord3f(1.0, 1.0, 1.0);
    glVertex2f(rect->x+rect->w, rect->y+rect->h);

    glTexCoord3f(0.0, 1.0, 1.0);
    glVertex2f(rect->x, rect->y+rect->h);
    glEnd();

    if(flush)
        SDL_GL_SwapBuffers();

    return 0;
}

GLuint load_texture(char *filename) {
    int x, y;
    FILE *file;
    unsigned char *imgdata;    
    GLuint texture;

    /* Allocate space for image. */
    texture = x = y = 32;
    imgdata = malloc(x * y * 3);
    
    if((file = fopen(filename, "r")) == NULL)
        return 0;

    fread(imgdata, x*y*3, 1, file);
    fclose(file);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //gluBuild2DMipmaps(GL_TEXTURE_2D, 3, x, y, GL_RGB, GL_UNSIGNED_BYTE, imgdata);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, imgdata);
    return texture;
}

int draw_level(SDL_Surface *glsurface, struct level *level, int xvp, int yvp) {
    int i;
    rect_t modr;
   
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
