/** @file
 * Base implmentation for flatland World, a 2d virtual machine World with
 * programmable dynamic objects and an external events system.
 */
#ifndef _WORLD_H
#define _WORLD_H

#include "worldobj.h"

#include <vector>
#include <string>

/** Flatland global namespace. */
namespace flatland {
    /** Possible instructions that can be ran thru the World vm. */
    enum WORLD_INSTRUCTIONS {
        IDLE,
        UP = 1, // As 1 because then we can send these as animation offsets as '0' is default.
        DOWN,
        LEFT,
        RIGHT
    };

    /**
     * Abstract event handler class provides an API for handleing events inside the vm
     * externally.
     */
    class WorldEventHandler {
    public:
        /**
         * Called when the vm has incremented its proc tick.
         */
        virtual void Tick() = 0;

        /**
         * Called when the vm has finished procing an object that
         * changed states and needs the specified coords updated with its bitmap.
         * @param object WorldObject object the coord update came from.
         * @param x X coord location of the object.
         * @param y Y coord location of the object.
         * @param TODO
         */
        virtual void CoordUpdate(WorldObject *object, int x, int y, int anim_offset) = 0;

        /**
         * Called when the vm finished running an object that has
         * changed states and needs the specified coords blanked.
         * @param object WorldObject the blank request came from.
         * @param x X coord.
         * @param y Y coord.
         */
        virtual void CoordBlank(WorldObject *object, int x, int y) = 0;
    };

    /**
     * Implements a 2d World/game engine with dynamic proccessable objects/items.
     */
    class World {
    public:
        const static int X = 20;
        const static int Y = 20;
    private:
        /** Proc tick counter. */
        unsigned long tick;
        /** Matrix of objects represented in the World. */
        WorldObject *grid[X][Y];
        /** Code outside of the vm to deal with external events. */
        WorldEventHandler *events;
    public:
        /**
         * Constructs a new World object.
         * @param events Event handler for external vm requests.
         */
        World(WorldEventHandler *events);

        /**
         * Deconstructs a World object.
         */
        ~World();

        /**
         * Proccesses the commands in a WorldObject object and modifies
         * the grid to reflect these changes.
         * @param x X current coord of the object.
         * @param y Y current coord of the object.
         */
        void ProcObject(int x, int y);

        /**
         * Execute the main proc loop to iterate/update the World.
         * @return 0 on success, 1 on fail. (probably will never return)
         */
        int RunLoop();

        /**
         * Add an object to the World.
         * @param obj object to add to the World.
         * @param x X coord to add the object in.
         * @param y Y coord to add the object in.
         * @return 0 on success, 1 on fail.
         */
        int AddObject(WorldObject *obj, int x, int y);
    };
}
#endif
