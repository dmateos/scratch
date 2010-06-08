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

import pygame, random, game_config

class OnScreenDisplay:
    """Renders a graphical/textual on screen display onto the given surface."""

    # Font to use for text rendering.
    font = None

    # Screen to render changes too.
    screen = None

    # Coords to start rendering at. 
    screenrect = []

    # Temp surface to build osd before bliting to display.
    surface = None    

    #underlay = None

    def __init__(self, screen, screenrect, fontsize):
        self.font = pygame.font.SysFont("dunno", fontsize)
        self.screen = screen
        self.screenrect = screenrect
        
        # Failed code for underlay for osd text, ill fix soon.
        #self.underlay = pygame.Surface((self.screenrect[2], self.screenrect[3]))
        #self.underlay.fill((0, 0, 0))
        #self.underlay.set_alpha(100)

        if game_config.DEBUG:
            print("Loaded an OSD at %dx%d" % (self.screenrect[0], self.screenrect[1]))

    def WriteText(self, text):
        """Update the surface buffer with new OSD data."""

        self.surface = self.font.render(text, True, (0, 0, 255))

    def RenderScreen(self):
        """Draw surface buffer to the screen."""

        #self.screen.blit(self.underlay, (self.screenrect[0], self.screenrect[1]))
        self.screen.blit(self.surface, (self.screenrect[0], self.screenrect[1]))


class LifeBar(OnScreenDisplay):
    """Renders a graphical life bar with adjustable levels to the surface."""

    # Life vars
    hp = 0
    hpmax = 0
    pixperlvl = 10

    # Under and over surface, under for maxhp imprint and over for
    # actual HP level.
    undersurf = None
    oversurf = None

    def __init__(self, screen, screenrect, fontsize, hp):
        OnScreenDisplay.__init__(self, screen, screenrect, fontsize)
        self.hp = hp
        self.hpmax = hp

    def DrawBar(self):
        """Render the lifebar."""

        # Draw Under surface
        self.undersurf = pygame.Surface((self.hpmax * self.pixperlvl, self.screenrect[3]))
        self.undersurf.fill((0, 0, 0))
        self.undersurf.set_alpha(100)

        # Draw HP surface
        self.oversurf = pygame.Surface((self.hp * self.pixperlvl, self.screenrect[3]))
        self.oversurf.fill((0, 255, 0))

        # Use our subclass's abilities to also write a description.
        self.WriteText()

    def RenderScreen(self):
        """Draw the lifebar to the screen."""

        self.screen.blit(self.undersurf, (self.screenrect[0], self.screenrect[1]))
        self.screen.blit(self.oversurf, (self.screenrect[0] , self.screenrect[1]))
        # Also call our subclass Render so we can get some text.
        OnScreenDisplay.RenderScreen(self)

    def ModHP(self, value):
        if value >= 0 and value <= self.hpmax:
            self.hp = value

    def WriteText(self):
        text = str(self.hp) + "/" + str(self.hpmax)
        OnScreenDisplay.WriteText(self, text)
