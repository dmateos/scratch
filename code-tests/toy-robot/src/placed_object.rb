class PlacedObject
  attr_reader :position

  def initialize(position)
    @position = position.clone
    @position.move
  end

  def ==(other)
    @position.x == other.x && @position.y == other.y
  end
end
