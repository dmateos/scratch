import pygame, random, npc

UP = 1
DOWN = 2
RIGHT = 3
LEFT = 4

DISTPERSTEP = 10
UPDATERATE = 1

class GameObject(pygame.sprite.DirtySprite):
    def __init__(self, color, pos, size, interaction_group):
        #Load an image for the sprite then set bounds based on it.
        pygame.sprite.DirtySprite.__init__(self)
        self.image = pygame.Surface(size)
        self.image.fill(color)
        self.rect = self.image.get_rect()
        self.rect.topleft = pos

        self.interaction_group = interaction_group

    def update(self):
        pass

                

