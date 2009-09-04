import pygame, gameobj, bullet

DIRPERSTEP = 10
class Player(gameobj.GameObject):
    def __init__(self, color, pos, size, interaction_group):
        gameobj.GameObject.__init__(self, color, pos, size, interaction_group)
        self.lastdir = 1
        self.NPC = True # For bullet class to know.

    def shoot(self):
        b = bullet.Bullet([255,255,255], self.rect.center, [2,2], self.interaction_group)
        b.set_direction(self.lastdir)
        return b

    def update(self, keypress):
        xpos, ypos = 0, 0

        #Set our position increment vars.
        if keypress == pygame.K_LEFT:
            xpos = -DIRPERSTEP
            self.lastdir = 1
        elif keypress == pygame.K_RIGHT:
            xpos = DIRPERSTEP
            self.lastdir = 2
        elif keypress == pygame.K_UP:
            ypos = -DIRPERSTEP
            self.lastdir = 3
        elif keypress == pygame.K_DOWN:
            ypos = DIRPERSTEP
            self.lastdir = 4

        #Mark as dirty and update the rect with the
        #new coords
        self.rect.move_ip(xpos, ypos)
        self.dirty = 1

        #Check for collisions and if so undo the above.
        for group in self.interaction_group:
            obsticle = pygame.sprite.spritecollide(self, group, False)
            if obsticle:
                if self.lastdir == 1:
                    self.rect.left = obsticle[0].rect.right
                elif self.lastdir == 2:
                    self.rect.right = obsticle[0].rect.left
                elif self.lastdir == 3:
                    self.rect.top = obsticle[0].rect.bottom
                elif self.lastdir == 4:
                    self.rect.bottom = obsticle[0].rect.top

