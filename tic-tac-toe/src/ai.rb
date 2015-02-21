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
    if board.set(@who, [rand(0..2), rand(0..2)]) == @who

    end 
  end
end
