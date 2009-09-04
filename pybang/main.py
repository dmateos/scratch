import sys, pygame, random, npc, tree, player, bullet

class PyBang:
    def __init__(self):
        self.tick = 0
        self.size = self.width, self.height = 900, 300    
   
        # Init SDL and other crap. 
        pygame.init()
        pygame.key.set_repeat(1, 1)
        self.screen = pygame.display.set_mode(self.size)
        self.background = pygame.image.load("resources/dirt.jpg")
        
        # Make some groups for our sprites, rendergrp is the global group
        # which sprites who want to actually be rendered must join.
        self.rendergrp = pygame.sprite.RenderUpdates()
        self.bullets = pygame.sprite.Group()
        self.trees = pygame.sprite.Group()
        self.npcs = npc.NPGroup()
        self.players = pygame.sprite.Group()  

        self.screen.blit(self.background, (0, 0))

    def Populate(self):
        count = 0
        while count < 10:
            treesize = random.randint(10, 40)
            treesp = tree.Tree([0, 255-random.randint(50, 150), 0], [random.randint(0, self.width), random.randint(0, self.height)], [treesize, treesize])
            self.trees.add(treesp)
            self.rendergrp.add(treesp)

            npcsp = npc.NPChar([255,0,0], [500, random.randint(0, self.height)], [10,10], self.trees)
            self.npcs.add(npcsp)
            self.rendergrp.add(npcsp)
       
            dummy = npc.DummyTarget([69, 69, 69], [random.randint(0, self.width), random.randint(0, self.height)], [10, 10], self.trees) # Dummy for targe practice ;p 
            self.npcs.add(dummy)
            self.rendergrp.add(dummy)

            count += 1

        self.playersp = player.Player([0, 0, 255], [0, 0], [10, 10], [self.trees, self.npcs])
        self.players.add(self.playersp)
        self.rendergrp.add(self.playersp)    

        pygame.display.flip()

    def RunLoop(self):
        while 1:
            #Get some window/keyboard input.
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    sys.exit()
                #Keyboard events
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_q:
                        sys.exit()
                    #Catch direction keys and move the player sprite accordingly.
                    elif event.key == pygame.K_UP or event.key == pygame.K_DOWN or event.key == pygame.K_LEFT or event.key == pygame.K_RIGHT:
                        self.players.update(event.key)
                    #Catch space key and shoot the players weapon
                    elif event.key == pygame.K_SPACE:
                        b = self.playersp.shoot()
                        self.bullets.add(b)
                        self.rendergrp.add(b)

            #Update the moving sprites.
            if not self.tick % npc.UPDATERATE: 
                self.npcs.update()
            if not self.tick % bullet.UPDATERATE:
                self.bullets.update()

            # Clear the last draw state and paint a new one with the updated sprites.
            self.rendergrp.clear(self.screen, self.background)
            reclist = self.rendergrp.draw(self.screen)

            pygame.display.update(reclist)

            pygame.time.wait(16*4)
            self.tick += 1


game = PyBang()
game.Populate()
game.RunLoop()

