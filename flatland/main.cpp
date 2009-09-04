/** @file
 * Main execution file, at the moment is primairly used to
 * glue-together to test/debug parts of the flatland API in development.
 * TODO:
 * Multibox objects, like maybe wobject.bind(object2, LEFT)
 */
#include "include/world.h"
#include "include/worldobj.h"
#include "include/screen.h"

#include <string.h>
#include <string>
#include <iostream>

#define DEBUG

using namespace flatland;
using namespace flatland::screen;

Screen *g_scr;
World *g_game;

void at_exit() {
    delete g_scr;
    delete g_game;
}

int main(int argc, char *argv[]) {
    Screen *scr = new Screen(World::X, World::Y, 30);
    ScreenEventHandler whandler(scr);
    World *game = new World(&whandler);

    g_scr = scr;
    g_game = game;
    atexit(at_exit);

    /* Some DrawableObjects. */
    DrawableObject red("red.bmp", true, 1);
    DrawableObject blue("blue.bmp", true, 2);
    DrawableObject blue_idle("blue.bmp", true, 1);
    DrawableObject sync_one("green.bmp", true, 1);
    DrawableObject sync_two("green.bmp", true, 1);
    DrawableObject sync_thr("green.bmp", true, 1);
    WorldObject object(1);

    scr->Init();

    red << LEFT << LEFT << LEFT << RIGHT << RIGHT << RIGHT;
    blue << UP << LEFT << DOWN << RIGHT;
    blue_idle << UP << IDLE << LEFT << IDLE << DOWN << IDLE << RIGHT << IDLE;
    sync_one << DOWN << DOWN << UP << UP;
    sync_two << DOWN << DOWN << UP << UP;
    sync_thr << DOWN << DOWN << UP << UP;
    object << IDLE;

    game->AddObject(&red, 10, 10);
    game->AddObject(&blue, 14, 14);
    game->AddObject(&blue_idle, 14, 16);
    game->AddObject(&sync_one, 3, 3);
    game->AddObject(&sync_two, 4, 3);
    game->AddObject(&sync_thr, 3, 2);
    game->AddObject(&object, 9, 9);

    game->RunLoop();
}
