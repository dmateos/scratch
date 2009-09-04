/** @file
 * Implements programmable renderable objects that can be placed in the
 * flatland world.
 */
#ifndef _WORLDOBJ_H
#define _WORLDOBJ_H

#include <vector>
#include <string>

/** Flatland global namespace. */
namespace flatland {
    /**
     * Represents a dynamic programable object to be represented in a world.
     */
    class WorldObject {
    private:
        /** How often the object instructions should be ran. */
        unsigned long tick_rate;
        /** Last update, to stop multiple updates in one turn. */
        unsigned long last_update;
        /** Pointer the the current instruction to run. */
        int instrptr;
        /** List of instructions. */
        std::vector<char> instructions;
    public:
        /**
         * Constructs a new WorldObject object instance.
         * @param update_freq This object should get proced every x ticks.
         */
        WorldObject(int update_freq);

        /** TODO **/
        virtual ~WorldObject() { }

        /**
         * Adds the instruction fed into object with << to
         * the instruction list.
         * @param val Value to add to instruction list.
         * @return Reference to this object.
         */
        WorldObject &operator<<(char val);

        /************************
        * A bunch of cookie cutter get/set methods for
        * some of the private variables.
        *************************/

        /**
         * Get how often this object should be updated.
         * @return update freqency value.
         */
        unsigned long get_tick_rate() const {
            return tick_rate;
        }

        /**
         * Get the value of the last tick this object was updated.
         * @return Tick last tick.
         */
        unsigned long get_last_update() const {
            return last_update;
        }

        /**
         * Get the current value of the instruction pointer.
         * @return Value of instruction pointer var.
         */
        int get_instructionptr() const {
            return instrptr;
        }

        /**
         * Get the instruction list.
         * @return List of instructions for this object.
         */
        std::vector<char>* get_instructions() {
            return &instructions;
        }

        /**
         * Set the value of the instruction pointer.
         * @param Value to set the instruction pointer too.
         */
        void set_instructionptr(int value) {
            instrptr = value;
        }

        /**
         * Set the last update paramater.
         * @param update Value of current tick?.
         */
        void set_last_update(int update) {
            last_update = update;
        }
    };
}
#endif
