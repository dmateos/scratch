/** @file
 * Wrapper around SDL library to draw WorldObject instances to a Screen
 * and collect keyboard/mouse? input from a player.
 */
#ifndef _SCREEN_H
#define _SCREEN_H

#include "world.h"
#include "worldobj.h"
#include <SDL/SDL.h>

#include <string>
#include <map>

namespace flatland {
    namespace screen {
        /**
         * Implements a WorldObject with extentions that allow it to be rendered to a
         * 2D Screen.
         */
        class DrawableObject : public WorldObject {
        private:
            /** Path of bitmap image to graphically represent object. */
            std::string bitmap;
            /** TODO */
            bool dir_animated;
        public:
            DrawableObject(std::string bitmap, bool direction_enabled, int tspeed) : WorldObject(tspeed), bitmap(bitmap), dir_animated(direction_enabled) { }

            std::string& get_bitmap() {
                return bitmap;
            }

            void set_bitmap(std::string bitmap) {
                bitmap = bitmap;
            }

            bool get_dir_animated() {
                return dir_animated;
            }
        };

        /**
         * Provides a Screen of pixels for rendering WorldObject's too.
         */
        class Screen {
        private:
            /** Underlying SDL_Surface to draw too. */
            SDL_Surface *surface;
            /** Bitmap cache, so we dont have to read from disk every time. */
            std::map<std::string, SDL_Surface*> bmp_cache;
            /** world grid X size. */
            const int X;
            /** World grid Y size. */
            const int Y;
            /** Square root? of the object size. */
            const int objsize;

            /**
             * Load a bitmap from the bitmap cache and/or disk if
             * cache read fails.
             * @param bitmap Path on disk or in cache of the image.
             * @return SDL surface pointer to bitmap.
             */
            SDL_Surface *LoadBitmap(std::string &bitmap);
        public:
            /**
             * Construct a new instance of Screen.
             * @param x size of world grid on x axis. (usually world::X)
             * @param y size of world grid on y axis. (usually world::Y)
             * @param ppo pixels per object.
             */
            Screen(int x, int y, int ppo);

            /**
             * Deconstruct Screen instance.
             */
            ~Screen();

            /**
             * Init the SDL subsystem.
             * @return 0 on success, 1 on fail.
             */
            int Init();

            /**
             * Poll keyboard and mouse for user I/O.
             * @return dunno yet.
             */
            int PollInput();

            /**
             * Render a WorldObject at the specified location reletive to
             * the Screen size.
             * @param object Object with bitmap to render.
             * @param x X grid location of object.
             * @param x Y grid location of object.
             * @param TODO
             */
            void RenderWorldObject(DrawableObject *object, int x, int y, int anim_offset);

            /**
             * Render a blank rectangle over a WorldObject bitmap at the location
             * specified.
             * @param x X grid location of obj.
             * @param y Y grid location of obj.
             */
            void RenderBlankObject(int x, int y);

            /**
             * Flush the Screen to update render changes.
             * @return Fuck knows.
             */
            int Flush();
        };

        /**
         * Pass events from the world virtual machine onto the Screen
         * for rendering.
         */
        class ScreenEventHandler : public WorldEventHandler {
        private:
            /** Screen object to send the events too. */
            Screen *scr;
        public:
            /**
             * Construct a new ScreenEventHandler.
             * @param TODO
             */
            ScreenEventHandler(Screen *surface) : WorldEventHandler(), scr(surface) { }

            /**
             *
             */
            void Tick() {
#ifdef DEBUG
                printf("end tick\n");
#endif
                if (scr != NULL) {
                    scr->PollInput();
                    scr->Flush();
                }
            }

            /**
             *
             */
            void CoordUpdate(WorldObject *object, int x, int y, int anim_offset) {
                if (scr != NULL) {
                    // Do a safe downcast to the child object using c++ dynamic_cast.
                    DrawableObject *dobject = dynamic_cast<DrawableObject*>(object);
                    if (dobject != NULL)
                        scr->RenderWorldObject(dobject, x, y, anim_offset);
                    // Fuck, this must not have originally been a DrawableObject,
                    // that can happen if other derived types are being added into the game
                    // map aswell.
                    else
                        printf("\tObject not renderable TODO\n");
                }
            }

            /**
             *
             */
            void CoordBlank(WorldObject *object, int x, int y) {
                if (scr != NULL)
                    scr->RenderBlankObject(x, y);
            }
        };
    }
}
#endif
