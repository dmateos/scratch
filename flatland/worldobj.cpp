/** @file
 * Implements programmable renderable objects that can be placed in the
 * flatland world.
 */
#include "include/worldobj.h"

#include <iostream>

using namespace flatland;

WorldObject::WorldObject(int update_freq)
        : tick_rate(update_freq), last_update(1), instrptr(0) {

}

WorldObject &WorldObject::operator<<(char val) {
    instructions.push_back(val);
    return *this;
}

