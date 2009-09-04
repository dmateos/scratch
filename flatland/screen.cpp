/** @file
 * Wrapper around SDL library to draw WorldObject instances to a Screen
 * and collect keyboard/mouse? input from a player.
 */
#include "include/screen.h"

#include <SDL/SDL.h>
#include <iostream>
#include <math.h>

using namespace flatland;
using namespace flatland::screen;

#define DEBUG //tmp

SDL_Surface *Screen::LoadBitmap(std::string &path) {
    // Try loading bitmap from cache, if not read from
    // disk and cache it for later.
    SDL_Surface *bmp = bmp_cache[path];
    if (bmp == NULL) {          //CLEAAN
        bmp = SDL_LoadBMP(path.c_str());
        if (bmp != NULL) {
            bmp_cache[path] = bmp;
#ifdef DEBUG
            printf("\tbitmap cache push: %s\n", path.c_str());
#endif
        } else {
#ifdef DEBUG
            printf("\tbitmap input fail: %s\n", path.c_str());
#endif
        }
    } else {
#ifdef DEBUG
        printf("\tbitmap cache hit: %s\n", path.c_str());
#endif
    }

    return bmp;
}

Screen::Screen(int x, int y, int ppo) : X(x), Y(y), objsize(ppo) {

}

Screen::~Screen() {
    // kill the bitmap cache.
    for (std::map<std::string, SDL_Surface*>::iterator iter = bmp_cache.begin(); iter != bmp_cache.end(); iter++) {
#ifdef DEBUG
        printf("bitmap free: %s\n", iter->first.c_str());
#endif
        SDL_FreeSurface(iter->second);
    }

    SDL_Quit();
}

int Screen::Init() {
    if ((SDL_Init(SDL_INIT_VIDEO) < 0) || (!(surface = SDL_SetVideoMode(X*objsize, Y*objsize, 16, SDL_SWSURFACE)))) {
        std::cout << "unable to init SDL " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_WM_SetCaption("Flatland", "Flatland");
#ifdef DEBUG
    printf("Screen init: %dx%d pixels, object cubes: %d^2\n", X*objsize,Y*objsize,objsize);
#endif
    return 0;
}

int Screen::PollInput() {
    SDL_Event event;
#ifdef DEBUG
    printf("Screen input poll......\n");
#endif

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            exit(0);
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_q:
                exit(0);
                break;
            default:
                break;
            }
        default:
            break;
        }
    }

    return 0;
}

void Screen::RenderWorldObject(DrawableObject *object, int x, int y, int anim_offset) {
    SDL_Surface *bitmap;
    SDL_Rect dest, frame;
#ifdef DEBUG
    printf("\tScreen render: %dx%d as %s, anim_offset: %d\n", x*objsize, y*objsize, object->get_bitmap().c_str(), anim_offset);
#endif
    // cant render if theres no image.
    if (!(bitmap = LoadBitmap(object->get_bitmap())))
        return;

    // Set the destination for the object on the Screen.
    object->get_dir_animated() ? frame.x = anim_offset * objsize : frame.x = 0;
    frame.y = 0;
    frame.w = objsize;
    frame.h = objsize;

    dest.x = x * objsize;
    dest.y = y * objsize;
    dest.w = objsize;
    dest.h = objsize;

    SDL_BlitSurface(bitmap, &frame, surface, &dest);
}

void Screen::RenderBlankObject(int x, int y) {
    SDL_Rect rect;

#ifdef DEBUG
    printf("\tScreen blank: %dx%d\n", x*objsize, y*objsize);
#endif
    rect.x = x * objsize;
    rect.y = y * objsize;
    rect.w = objsize;
    rect.h = objsize;

    SDL_FillRect(surface, &rect, 0);
}

int Screen::Flush() {
#ifdef DEBUG
    printf("Screen flush\n");
#endif
    SDL_UpdateRect(surface, 0, 0, X*objsize,Y*objsize);
    return 0;
}
