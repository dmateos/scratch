#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include "render.h"

int main(int argc, char *argv[]) {
    Screen *screen;
    rect_t r1,r2,r3;

    screen = init_gfx();

    r1 = build_rect(0, 0, 10, 10, 1.0, 0, 0);
    r2 = build_rect(100, 100, 10, 10, 0, 1.0, 0);
    r3 = build_rect(200, 200, 10, 10, 0, 0, 1.0);
    draw_rects(screen, 1, &r1, &r2, &r3, NULL);

    while(1) {
        r1.x += 1;
        r2.x += 1;
        r3.x += 1;
        clear_screen(screen);
        draw_rects(screen, 1, &r1, &r2, &r3, NULL);
        SDL_Delay(1000);
    }

    return 0;
}

