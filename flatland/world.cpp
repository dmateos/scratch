/** @file
 * Base implmentation for flatland World, a 2d virtual machine World with
 * programmable dynamic objects and an external events system.
 */

#include "include/world.h"

#include <string.h>
#include <unistd.h>
#include <iostream>

#define DEBUG //temp
using namespace std;
using namespace flatland;

World::World(WorldEventHandler *events) : tick(0), events(events) {
    memset(grid, '\0', sizeof grid);
    printf("FLATLAND ENGINE, size: %dx%d, speed: TODO\n", X, Y);
}

World::~World() {

}

void World::ProcObject(int x, int y) {
    /* Get the next instruction pointed to by the objects instructionptr and do some stuff. */
    WorldObject *object = grid[x][y]; //should check this.
    char instruction = object->get_instructions()->at(object->get_instructionptr());
#ifdef DEBUG
    printf("proc object: %dx%d, tick_rate:%ld, last_tick:%ld\n", x, y, object->get_tick_rate(), object->get_last_update());
    printf("\tinstruction: %d, instructionptr: %d/%ld\n", instruction, object->get_instructionptr(), object->get_instructions()->size()-1);
#endif

    switch (instruction) {
    default:
    case IDLE:
        events->CoordUpdate(object, x, y, IDLE);
        break;

        /* For each direction we check if where we move isn't off the wall or null, move then fire events. */
    case UP:
        if ((y > 0) && (grid[x][y-1] == NULL)) {
            grid[x][y-1] = object;
            grid[x][y] = NULL;
            events->CoordBlank(object, x, y);
            events->CoordUpdate(object, x, y-1, UP);
        }
        break;
    case LEFT:
        if ((x > 0) && (grid[x-1][y] == NULL)) {
            grid[x-1][y] = object;
            grid[x][y] = NULL;
            events->CoordBlank(object, x, y);
            events->CoordUpdate(object, x-1, y, LEFT);
        }
        break;

        /* Right and down are special cases because they move in the direction of not yet
          * processed objects so we have to look ahead a bit first. */
    case RIGHT:
        if (x+1 < X) {
            if (grid[x+1][y] != NULL)
                ProcObject(x+1, y);

            if (grid[x+1][y] == NULL) {
                grid[x+1][y] = object;
                grid[x][y] = NULL;
                events->CoordBlank(object, x, y);
                events->CoordUpdate(object, x+1, y, RIGHT);
            }
        }
        break;
    case DOWN:
        if (y+1 < Y) {
            if (grid[x][y+1] != NULL)
                ProcObject(x, y+1);

            if (grid[x][y+1] == NULL) {
                grid[x][y+1] = object;
                grid[x][y] = NULL;
                events->CoordBlank(object, x, y);
                events->CoordUpdate(object, x, y+1, DOWN);
            }
        }
        break;
    }
    /* If we are at the end of the instruction list, reset the pointer to zero. */
    if (static_cast<unsigned int>(object->get_instructionptr()) == object->get_instructions()->size()-1)
        object->set_instructionptr(0);
    else
        object->set_instructionptr(object->get_instructionptr()+1);

#ifdef DEBUG
    printf("end object proc\n");
#endif
    object->set_last_update(tick);
}

int World::RunLoop() {
    WorldObject *obj;

    /* Main game proc loop. */
    while (true) {
#ifdef DEBUG
        printf("\ntick %ld\n", tick);
#endif
        for (int x = 0; x < X; x++) {
            for (int y = 0; y < Y; y++) {
                if ((obj = grid[x][y])) {
                    if ((!(tick % obj->get_tick_rate()) && (obj->get_last_update() != tick))) {
                        ProcObject(x, y);
                    }
                }
            }
        }
        tick++;
        events->Tick();
        sleep(1);
    }
}

int World::AddObject(WorldObject *obj, int x, int y) {
    grid[x][y] = obj;
    events->CoordUpdate(obj, x, y, 0);
    return 0;
}
