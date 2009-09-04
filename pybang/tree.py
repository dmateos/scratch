import pygame

class Tree(pygame.sprite.Sprite):
    def __init__(self, color, pos, size):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface(size)
        self.image.fill(color)

        self.rect = self.image.get_rect()
        self.rect.topleft = pos

    def update(self):
        pass
