class Table
  attr_reader :xmax, :ymax, :placed_objects

  def initialize(xmax, ymax)
    @xmax = xmax;
    @ymax = ymax;
  end

  def still_on_surface?(pos)
    (pos.x >= 0 && pos.x <= xmax && pos.y >= 0 && pos.y <= ymax) if !pos.x.nil? && !pos.y.nil?
  end
end
