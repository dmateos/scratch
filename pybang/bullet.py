import pygame, random, npc, gameobj

UPDATERATE = 1
DISTPERSTEP = 10

class Bullet(gameobj.GameObject):
    def __init__(self, color, pos, size, interaction_group):
        gameobj.GameObject.__init__(self, color, pos, size, interaction_group)
        self.stopped = False
        self.direction = 1

    def set_direction(self, direction):
        self.direction = direction

    def update(self):
        xpos, ypos = 0, 0

        #If weve been stopped bail out now.
        if self.stopped:
            return

        #Switch on the direction variable and increment, deincrment the
        #pos axis vars 1 for left, 2 for right, 3 for up etc... 
        if self.direction == 1:
            xpos =- DISTPERSTEP
        elif self.direction == 2:
            xpos = DISTPERSTEP
        elif self.direction == 3:
            ypos =- DISTPERSTEP
        elif self.direction == 4:
            ypos = DISTPERSTEP

        #Update the actual rectangle of the object and increment the
        #roam count so we dont move in one direction forever. 
        self.rect.move_ip(xpos, ypos)
        self.dirty = 1
 
        #Do some bounds checking against all other sprites in our 'friends' groups and
        #stop if we hit something.
        for thing in self.interaction_group:
            # Get the collisions
            collides = pygame.sprite.spritecollide(self, thing, False)
           
            # Fuck we have some so we stop the bullet.
            if collides:     
                self.rect.move_ip(-xpos, -ypos)
                self.dirty = 0
                self.stopped = True
               
                # Special cases to do things depending on what object we hit
                # if we hit a npc we need to hurt them
                if isinstance(thing, npc.NPGroup): 
                    for collide in collides:
                        collide.hurt()
                        print("NPC is hurt %d/%d") % (collide.HP, npc.HPMAX)


                

