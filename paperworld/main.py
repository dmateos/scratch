#! /usr/bin/python
# This file is part of PaperWorld.
# All code, artwork, and sound copyright (C) 2009, Daniel Mateos
#
# PaperWorld is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# PaperWorld is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with PaperWorld.  If not, see <http://www.gnu.org/licenses/>.

import pygame, game_level, game_sprites, game_osd, game_config, gc

def print_usage():
    print "PaperWorld"
    print "In game keys:"
    print "    left/right arrows to move"
    print "    space to jump"
    print "    b for debug screens"
    print """    j/k to manually span viewport (broken if not softscroll
                 and even then youre fighting it))"""
    print "    l to toggle soft scrolling"


# Do some pygame init stuff.
pygame.init()
screen = pygame.display.set_mode(game_config.screensize)
pygame.key.set_repeat(50, 1)
stepping = 4
print_usage()

# Garbage collection debug code.
if game_config.GC_DEBUG_LEAK:
    gc.set_debug(gc.DEBUG_LEAK | gc.DEBUG_STATS)

# Load a player and map objects.
player = game_sprites.Player([50, game_config.height-139, 10, 10], screen)
map = game_level.Level(screen, [game_config.width, game_config.height], [0, 0], player)

# Add an OSD to show some stats.
osd1 = game_osd.OnScreenDisplay(screen, [5, 5, 300, 50], 20)
osd2 = game_osd.OnScreenDisplay(screen, [5, 20, 300, 30], 20)
lifebar = game_osd.LifeBar(screen, [10, game_config.height-40, 40, 20], 30, 20)

running = True
clock = pygame.time.Clock()
softcent = False

# Run loop and start parsing input data.
while running:
    evt =  pygame.event.poll()

    if evt.type == pygame.QUIT:
        running = False

    elif evt.type == pygame.KEYDOWN:
        keys = pygame.key.get_pressed()

        if evt.key == pygame.K_q:
            running = False
        # Some keys need to catch the keypress and check for other keys
        # also down.
        elif evt.key == pygame.K_LEFT:
            # If we just Center on every player move we get the effect of
            # following him AND bonus stops when he gets to edges.
            map.player.move(-stepping, 0)
            if keys[pygame.K_SPACE]:
                map.player.jump(stepping)

        elif evt.key == pygame.K_RIGHT:
            map.player.move(stepping, 0)
            if keys[pygame.K_SPACE]:
                map.player.jump(stepping)

        elif evt.key == pygame.K_SPACE:
            map.player.jump(stepping)
            if keys[pygame.K_RIGHT]:
                map.player.move(stepping, 0)
            elif keys[pygame.K_LEFT]:
                map.player.move(-stepping, 0)

        elif evt.key == pygame.K_b:
            game_config.DEBUG = True if game_config.DEBUG == False else False

    # Update map objects, center on the player and render the level + OSD.
    map.update()
    map.CenterViewX(map.player)
    #map.CenterViewXSoft(map.player)
    map.RenderLevel()

    lifebar.ModHP(map.player.health)
    lifebar.DrawBar()
    lifebar.RenderScreen()

    # OMG dead
    if map.player.health == 0:
        map.player.reset()

    if game_config.DEBUG:
        osd1.WriteText("fps: %d, objcnt: %d/%d, pcol:%s, vp: %d-%dx%d-%d, pw: %dx%d"
                            % (clock.get_fps(), map.objrcount, 
                               map.leveldata.objcount, map.player.lastcol,
                               map.viewport.left, map.viewport.left + map.viewport.w,
                               map.viewport.top, map.viewport.top + map.viewport.h,
                               map.player.worldrect.left, map.player.worldrect.top))

        osd2.WriteText("pstate: %s" % map.player.state)

        osd1.RenderScreen()
        osd2.RenderScreen()

    # Flip the screen and tick the clock.
    pygame.display.flip() 
    clock.tick(60)

