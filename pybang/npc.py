import pygame, random, gameobj

ROAMMAX = 5
HPMAX = 50
UPDATERATE = 5
DISTPERSTEP = 10

class NPGroup(pygame.sprite.Group):
    def __init__(self):
        pygame.sprite.Group.__init__(self)

class NPChar(gameobj.GameObject):
    updaterate = 5

    def __init__(self, color, pos, size, interaction_group):
        
        #Load an image for the sprite then set bounds based on it.
        gameobj.GameObject.__init__(self, color, pos, size, interaction_group)

        self.direction = random.randint(1,6)
        self.roamcount = 0
        self.roamdist = random.randint(1, ROAMMAX)
        self.HP = HPMAX

    def roam(self):
        xpos, ypos = 0, 0

        #Switch on the direction variable and increment, deincrment the
        #pos axis vars 1 for left, 2 for right, 3 for up etc... 
        if self.direction <= 4:
            xpos =- DISTPERSTEP
#        elif self.direction == 4:
#            xpos = DISTPERSTEP
        elif self.direction == 5:
            ypos =- DISTPERSTEP
        elif self.direction == 6:
            ypos = DISTPERSTEP

        #Update the actual rectangle of the object and increment the
        #roam count so we dont move in one direction forever. 
        self.rect.move_ip(xpos, ypos)
        self.roamcount += 1
        self.dirty = 1 

        #Do some bounds checking against all other sprites in our 'friends' groups
        #and reverse the changes if we have bumped into shit. 
        if pygame.sprite.spritecollideany(self, self.interaction_group):
            self.rect.move_ip(-xpos, -ypos)
            self.roamcount == self.roamdist
            self.dirty = 0
       
        #This is where we chose a new direction if we have reached our roam dist
        #or have bumped into another sprite. 
        if self.roamcount == self.roamdist:
            self.direction = random.randint(1,6)
            self.roamcount = 0
            
    def hurt(self):
        self.HP -= 10
        if self.HP == 0:
            self.kill()

    def update(self):
        self.roam()

class DummyTarget(NPChar):
    def __init__(self, color, pos, size, interaction_group):
        NPChar.__init__(self, color, pos, size, interaction_group)

    def roam(self):
        pass
