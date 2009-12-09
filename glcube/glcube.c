#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/glu.h>

#define X 90
#define Y 90

void draw_scene(SDL_Surface *surface) {
    glLoadIdentity();
    glColor3f(0.5f,0.5f,1.0f);
    glTranslatef(0.0f,0.0f,-6.0f);
    glBegin(GL_QUADS);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glVertex3f( 1.0f, 1.0f, 0.0f);
        glVertex3f( 1.0f,-1.0f, 0.0f);
        glVertex3f(-1.0f,-1.0f, 0.0f);
    glEnd();
    SDL_GL_SwapBuffers();
}

int main(int argc, char **argv) {
    SDL_Surface *display;
    SDL_Event event;
    int mainloop;

    mainloop = 1;

    /* Init SDL, display and openGL. */
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return -1;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    if(!(display = SDL_SetVideoMode(800, 600, 16, SDL_OPENGL)))
        return -1;

    /* opengl 2d init stuff. */
    glClearDepth(1.0);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)800/(GLfloat)600, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(1.0, 1.0, 1.0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    while(mainloop) {
        draw_scene(display);
        sleep(1000);
        /* user io capture. */
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
                        default:
                            break;
                    }
                break;
            }
        }
    }
    SDL_Quit();
    return 0;
}
