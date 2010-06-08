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

import os, pygame, random, game_config

# Temp, quick cache to speed up image loads.
imgcache = dict()

class GameObject:
    """Generic object to represent a mappable in game object."""

    # Level co-ords, x,y,w,h
    worldrect = []

    # State used to implement object state changes between ticks like
    # jumping, moving right etc...
    state = None

    image = None

    # Records last move, important for collision fixing.
    lastmove = [0, 0]

    anim_frames = 0
    frame_width = 0
    frame_height = 0
    frame_count = 0
    frame_skip = 10
    frame_last = None

    def __init__(self, worldrect, image, screen, anim_frames=0, frame_width=0, frame_height=0):
        try:
            self.image = imgcache[image]
        except KeyError:
            self.image = pygame.image.load(os.path.join("resources/", image)).convert()
            imgcache[image] = self.image
            print("cached", image)
            print("x%d y%d" %(self.image.get_rect().w, self.image.get_rect().h))

        self.anim_frames = anim_frames
        self.frame_width = frame_width        
        self.frame_height = frame_height

        # Build the worldrect out of supplied coords and deduced object size.
        self.worldrect = pygame.Rect([worldrect[0], worldrect[1], 
                                     self.image.get_rect().w, self.image.get_rect().h])

    def move(self, x, y):
        self.worldrect.move_ip(x, y)
        self.lastmove = [x, y] 

    def anim_image(self):
        if self.anim_frames == 0:
            return self.image

        self.frame_skip += 1

        if not self.frame_skip % 5 or self.frame_last == None:
            image = self.image.subsurface([self.frame_count*self.frame_width, 0, self.frame_width, 298])
            image.set_colorkey((245,217,237))
            if self.frame_count < 2:
                self.frame_count += 1
            else: 
                self.frame_count = 0

            self.frame_last = image
            return image
        else:
            return self.frame_last

class Player(GameObject):
    """Player class for a single physical player."""

    # Variables to controll player jumping semantics.
    jumpcount = 0
    jumpmax = 20
    jumpstep = 4

    # Records last collision for debugging at the mome.
    lastcol = []
    health = 20
    startpos = []

    state = {"MOVE-RIGHT" : 0, "MOVE-LEFT" : 0, "JUMP-UP" : 0, "JUMP-DOWN" : 0}

    def __init__(self, worldrect, screen):
        self.startpos = worldrect
        GameObject.__init__(self, worldrect, "batman.gif", screen)

    def update(self):
        """ Check the players state and perform the apropriate update."""

        if self.state["JUMP-UP"]:
            self.worldrect.move_ip(0, -self.jumpstep)
            self.jumpcount += 1

            # Here we check if our jumpcount has reached the jump max, if so 
            # its time to switch to JUMP-DOWN state so we dont anger newton.
            if self.jumpcount >= self.jumpmax:
                self.state["JUMP-DOWN"] = True
                self.state["JUMP-UP"] = False

        elif self.state["JUMP-DOWN"]:
            self.worldrect.move_ip(0, self.jumpstep)
            self.jumpcount -= 1

            # Same deal, if we hit the floor from last jump clear the state.
            if self.jumpcount <= 0:
                self.state["JUMP-DOWN"] = False

    def collide(self, list, object):
        self.lastcol = list

        # IDEA: CHECK DIRECTINS + COLLISION DIRECTIOn to deduce :D 

        # If the list is none we arnt hitting anything, even the floor
        # so we should probably fall or we shall anger newton here also. 
        if list == None:
            if not self.state["JUMP-UP"] and not self.state["JUMP-DOWN"]:
                self.worldrect[1] += 4
            return

        # Is this a monster? if so ouch.
        # Better way to do this plz. 
        if isinstance(object, Monster):
            self.health -= 1
            return

        # Check for left/right collides, also have to exclude bottom as 
        # if were not falling we should always be hitting it.
        if ("BOTTOM" not in list and "TOP" not in list) and ("RIGHT" in list or "LEFT" in list):
            self.move(-self.lastmove[0], self.lastmove[1])

        # Stop jumping if we find a floor. 
        if "BOTTOM" in list and self.state["JUMP-DOWN"]:
            self.state["JUMP-DOWN"] = False
            #self.worldrect[1] -= 1
            self.jumpcount = 0
        elif "TOP" in list and self.state["JUMP-UP"]:
            self.state["JUMP-UP"] = False
            self.jumpcount = 0
            self.worldrect[1] += 5

    def jump(self, step):
        """Set the player state to jump if no other state is active."""

        if not self.state["JUMP-DOWN"]:
            self.state["JUMP-UP"] = True
            self.jumpstep = step

    def reset(self):
        self.worldrect[0] = self.startpos[0]
        self.worldrect[1] = self.startpos[1]
        self.health = 20


class Monster(GameObject):
    """Monster class for world NPC's."""

    # Vars to keep track of its movement, some very dumb
    # AI which will be killed soon.
    stepcount = 0
    roamstep = 1
    roammax = 150

    def __init__(self, worldrect, image, screen, anim_frames=0, frame_width=0, frame_height=0):
        # Same deal as in player. 
        GameObject.__init__(self, worldrect, image, screen, anim_frames, frame_width, frame_height)
        self.roammax = random.randint(75, self.roammax)

    def update(self):
        # Do some basic moveing around the level.
        if self.state == "MOVE_RIGHT":
            self.worldrect.move_ip(self.roamstep, 0)
            self.stepcount += 1

            if self.stepcount >= self.roammax:
                self.state = "MOVE_LEFT"

        elif self.state == "MOVE_LEFT":
            self.worldrect.move_ip(-self.roamstep, 0)
            self.stepcount -= 1

            if self.stepcount <= 0:
                self.state = "MOVE_RIGHT"

class Tile(GameObject):
    """Tile class for static world objects."""

    def __init__(self, worldrect, screen):
        GameObject.__init__(self, worldrect, "levels/red.gif", screen)

