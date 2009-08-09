import sys, pygame, random
screensize = [800, 600]

class TreeNode:
    def __init__(self, (x, y, w, h)=(0, 0, 10, 100)):
        self.left = None
        self.right = None
        self.up = None

        self.height = h
        self.width = w

        self.x = x
        self.y = y
        self.rect = [x, y, w, h]

        self.color = [0, 255, 0]

    def add_left_node(self):
        if self.left == None:
            self.left = TreeNode((self.x-50, self.y/2, 50, 10))
            return self.left

    def add_right_node(self):
        if self.right == None:
            self.right = TreeNode((self.x+100, self.y/2, 50, 10))
            return self.right

    def add_ext_node(self):
        if self.up == None:
            self.up = TreeNode((self.x, self.y-50, 10, 50))
            return self.up

class Tree:
    def __init__(self, canvas):
        self.termnode = TreeNode((screensize[0]/2, screensize[1]-10, 10, 50))

    def do_tree_op_all(self, op):
        """Call a function every branch in tree."""
        self._do_tree_op_all(op, self.termnode)

    def _do_tree_op_all(self, op, treenode):
        """Call a function on every branch in tree starting from treenode."""

        if treenode.left != None:
            self._do_tree_op_all(op, treenode.left)
        if treenode.right != None:
            self._do_tree_op_all(op, treenode.right)
        if treenode.up != None:
            self._do_tree_op_all(op, treenode.up)

        op(treenode)


screen = pygame.display.set_mode(screensize)
def render_tree(treenode):
    nodeimg = pygame.Surface((treenode.width, treenode.height))
    nodeimg.fill(treenode.color)
    screen.blit(nodeimg, treenode.rect)

def grow_branch(treenode):
    chance = random.randint(1, 7)

    if chance == 1:
        treenode.add_left_node()
    elif chance == 2:
        treenode.add_right_node()
    elif chance > 3 and chance < 7:
        treenode.add_ext_node()

def set_color(treenode):
    treenode.color = [100, 0, 0]

def main():
    pygame.init()
    tree = Tree(screen)

    while True:
        tree.do_tree_op_all(render_tree)
        pygame.display.update()

        # Mutate tree for next time.  
        tree.do_tree_op_all(grow_branch)

        pygame.time.wait(1000)

main()
