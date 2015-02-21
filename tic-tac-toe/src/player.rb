class Player
  def initialize(who)
    @who = who
  end

  def play(board)
    move = parse_move
    until board.set(@who, move) == @who do
      move = parse_move
    end
  end

  private
  def parse_move
    gets.split(",").map(&:to_i)
  end
end
