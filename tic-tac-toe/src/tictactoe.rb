require_relative "board"
require_relative "player"
require_relative "ai"

class TicTacToe
  attr_reader :board, :players

  def initialize(board, players)
    @board = board
    @players = players
  end

  def play
    until board.winner? do
      @players.each do |player|
        player.play(@board) unless @board.winner?
      end
      @board.print
    end
  end
end

#game = TicTacToe.new(Board.new, [Player.new(:x), RandomAI.new(:o)])
game = TicTacToe.new(Board.new, [RandomAI.new(:x), RandomAI.new(:o)])
game.play
