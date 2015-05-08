class Table
  attr_reader :xmax, :ymax, :placed_objects

  def initialize(xmax, ymax)
    @xmax = xmax;
    @ymax = ymax;
    @placed_objects = []
  end

  def still_on_surface?(pos)
    (pos.x >= 0 && pos.x <= xmax && pos.y >= 0 && pos.y <= ymax) if !pos.x.nil? && !pos.y.nil?
  end

  def place_object(obj)
    @placed_objects << obj
  end

  def map(robot = nil)
    char_map = { north: '^', south: 'v', east: '>', west: '<' }
    output = ""

    @ymax.downto(0) do |y|
      0.upto(@xmax) do |x|
        if !robot.nil? && robot.position == Position.new(x, y, robot.position.orientation)
          output += "[#{char_map[robot.position.orientation]}]"
        elsif collide_with_object?(Position.new(x, y, nil))
          output += "[X]" 
        else
          output += "[ ]"
        end
      end
      output += "\n"
    end
    output
  end

  def collide_with_object?(pos)
    @placed_objects.include?(pos)
  end
end
