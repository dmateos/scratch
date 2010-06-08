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

import sys, os, random, ConfigParser, pygame, game_sprites, game_config
 
class Level:
    """Level class manages backgrounds, tiles, rendering, updateing objects and scrolling within the virtual world."""

    # Level data loaded from config file, backgrounds, tilemaps, worldsize etc.
    leveldata = None
 
    # SDL screen to render viewport too. (also size of viewport)
    screen = None

    # Viewport coords. (The window in the level) 
    viewport = []

    # Player and monster objects.
    player = None

    objrcount = 0

    def __init__(self, screen, screensize, vp_coords, player):
        self.screen = screen
        self.player = player
        self.viewport = pygame.Rect([vp_coords[0], vp_coords[1], screensize[0], screensize[1]])

        # Create a level data object and load some data.
        self.leveldata = LevelData(screen)
        self.leveldata.LoadData("levels/level1.conf")

    def update(self):
        """Update level interactive objects."""

        for monster in self.leveldata.monsters:
            monster.update()

        # Do the player update, check collisions and ask for another update if so.
        self.player.update()
        list, obj = self.CheckCollides(self.player, self.leveldata.tileobjects + self.leveldata.monsters)
        self.player.collide(list, obj)

    def RenderLevel(self):
        """Render the background, tiles, npcs and player to the screen."""
 
        # Create a subbackground within the current viewport and blit it to the screen.
        # TODO: wrap this on horiz/vert cross over.
        subbackground = self.leveldata.background.subsurface(self.viewport)
        self.screen.blit(subbackground, (0, 0))
        self.objrcount = 0
 
        # Render tiles to the screen.
        for tile in self.leveldata.tileobjects:
            if self.CheckOnScreen(tile):
                self.screen.blit(tile.image, (tile.worldrect.left - self.viewport.left, tile.worldrect.top - self.viewport.top))
                self.objrcount += 1

        # Blit all the monsters to the screen based on their world coord and current offset from the viewport.
        for monster in self.leveldata.monsters:
            self.screen.blit(monster.anim_image(), (monster.worldrect.left - self.viewport.left, monster.worldrect.top - self.viewport.top))
            self.objrcount += 1

        # Blit the player to the screen, player is drawn based on its screen coords not world coords.
        self.screen.blit(self.player.image, (self.player.worldrect.left - self.viewport.left, self.player.worldrect.top - self.viewport.top))
    
    def CenterViewX(self, object):
        """Center the screen so the object specified is in the middle."""

        # Get the location of the object to center on and move back
        # half the screensize so its middled. 
        newloc = object.worldrect.left - (self.viewport.w / 2)

        # Do some checks to make sure we wont go over the screen edge,
        # if we do render as close as possible to edge. 
        if newloc <= 0:
            self.viewport.left = 0
        elif newloc >= (self.leveldata.worldsize.w - self.viewport.w):
            self.viewport.right = self.leveldata.worldsize.w
        else: 
            self.viewport.left = newloc

    def CenterViewXSoft(self, object):
        newloc = object.worldrect.left - (self.viewport.w / 2)

        if newloc <= 0:
            if self.viewport.left > 0:
                self.viewport.move_ip(-2, 0)
        elif newloc >= (self.leveldata.worldsize.w - self.viewport.w):
            if self.viewport.right < self.leveldata.worldsize.w:
                self.viewport.move_ip(2, 0)
        else:
            # If soft enabled, slowly make our way to the object each call.
            # Deduce what direction to go.
            if self.viewport.left < newloc:
                self.viewport.move_ip(2, 0)
            elif self.viewport.left > newloc:
                self.viewport.move_ip(-2, 0)

    def CheckCollide(self, object, object2):
        """Implement me"""
        colrect = object.worldrect.colliderect(object2.worldrect)
        hitlist = []

        if colrect:
            if object2.worldrect.left < object.worldrect.left:
                hitlist.append("LEFT")
            if object2.worldrect.right > object.worldrect.right:
                hitlist.append("RIGHT")
            if object2.worldrect.top < object.worldrect.top:
                hitlist.append("TOP")
            if object2.worldrect.bottom > object.worldrect.bottom:
                hitlist.append("BOTTOM")

        return hitlist

    def CheckCollides(self, object, objects):
        for obj in objects:
            list = self.CheckCollide(object, obj)
            if len(list) > 0:
                return list, obj
        return None, None

    def CheckOnScreen(self, object):
        """Checks if an object is on the screen or not."""

        # Check against left and right screen edges for X coord.
        if (object.worldrect.right - self.viewport.left) < 0:
            return False
        elif (object.worldrect.left - self.viewport.left) > self.viewport.w:
            return False
        else: return True

class LevelData:
    """Handles loading level data from disk and parsing it into useable structures."""
    # Config file, in python ConfigFile format.
    configfile = None
    
    # Path to background for this level.
    backgroundpath = None
    
    # Actual background image data.
    background = None

    # World size vars.
    worldsize = []

    # Tile size vars.
    tilesize = []

    # Positions of the tiles on the map.
    tileposmap = None

    tileobjects = []

    monsters = []

    objcount = 0

    # Needed for making monster objects, TODO why?
    screen = None

    def __init__(self, screen):
        self.screen = screen

    def LoadData(self, filename):
        self.configfile = ConfigParser.ConfigParser()
        self.configfile.read(os.path.join("resources/", filename))
        self.GetValues()

    def GetValues(self):
        # Load the world dimentions and background.
        worldsizeX = self.configfile.getint("Map Data", "worldsizeX")
        worldsizeY = self.configfile.getint("Map Data", "worldsizeY")
        self.worldsize = pygame.Rect([0, 0, worldsizeX, worldsizeY])

        self.backgroundpath = self.configfile.get("Map Data", "background")
        self.background = pygame.image.load(os.path.join("resources/", self.backgroundpath)).convert()

        # Get the tile data.
        tilesizeX = self.configfile.getint("Map Tiles", "tilesizeX")
        tilesizeY = self.configfile.getint("Map Tiles", "tilesizeY")
        self.tilesize = pygame.Rect([0, 0, tilesizeX, tilesizeY])
        
        # Parse from new line delimited array of tiles to an array of chars.
        self.tileposmap = self.configfile.get("Map Tiles", "tiledata").split("\n")
        self.tilebitmaps = dict(self.configfile.items("Tile Map"))

        y = 0
        for tile_lines in self.tileposmap:
            x = 0
            for c in tile_lines:
                if c != "0":
                    self.tileobjects.append(game_sprites.Tile([(x * self.tilesize.w), (y * self.tilesize.h), self.tilesize.w, self.tilesize.h], self.screen))
                    self.objcount += 1
                x += 1
            y += 1

        # Get monster list.
        self.monsterlist = dict(self.configfile.items("Monster List")).values()

        # Iterate through monster list and load monster objects from values.
        for monster in self.monsterlist:
            mdata = dict(self.configfile.items(monster))
            mobj = game_sprites.Monster([int(mdata["x"]), int(mdata["y"])], 
                                        os.path.join("levels/", mdata["image"]), self.screen, 
                                        int(mdata["anim_frames"]), int(mdata["frame_width"]), int(mdata["frame_height"]))
            mobj.state = "MOVE_RIGHT"
            self.monsters.append(mobj)
            self.objcount += 1

            if game_config.DEBUG:
                print mdata

        if game_config.DEBUG:
            print(self.backgroundpath, self.worldsize, self.tilesize, self.tileposmap, self.tilebitmaps)
