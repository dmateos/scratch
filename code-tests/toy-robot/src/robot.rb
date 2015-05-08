require_relative 'position'
require_relative 'table'

class Robot
  attr_reader :position

  def initialize(table)
    @table = table
    @position = NilPosition.new
  end

  def place(position)
    position = Position.new(*position.split(','))

    if @table.still_on_surface?(position) && position.orientation_valid?
      @position = position
    end
  end

  def move
    pos = @position.clone
    pos.move
    @position.move if @table.still_on_surface?(pos) && !@table.collide_with_object?(pos)
  end

  def left
    @position.turn_left
  end

  def right
    @position.turn_right
  end

  def report(console = true)
    puts @position.to_s if console && !position.to_s.nil?
    @position.to_s
  end
end
