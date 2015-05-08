class RandomAI
  def initialize(who)
    @who = who
  end

  def play(board)
    until board.set(@who, [rand(0..2), rand(0..2)]) == @who do
      nil
    end
  end
end

class SmartAI
  def initialize(who)
    @who = who
  end

  def play(board)
    # horizontal left
    board.set(@who, [0,2]) if board.grid[0][0] == :x and board.grid[0][1] == :x
    board.set(@who, [1,2]) if board.grid[1][0] == :x and board.grid[1][1] == :x
    board.set(@who, [2,2]) if board.grid[2][0] == :x and board.grid[2][1] == :x

    #horizontal right
    board.set(@who, [0,0]) if board.grid[0][1] == :x and board.grid[0][2] == :x
    board.set(@who, [1,0]) if board.grid[1][1] == :x and board.grid[1][2] == :x
    board.set(@who, [2,0]) if board.grid[2][1] == :x and board.grid[2][2] == :x

    #vertical left
    board.set(@who, [2,0]) if board.grid[0][0] == :x and board.grid[1][0] == :x
    board.set(@who, [2,1]) if board.grid[0][1] == :x and board.grid[1][1] == :x
    board.set(@who, [2,2]) if board.grid[0][2] == :x and board.grid[1][2] == :x

    #vertial right
  end
end
