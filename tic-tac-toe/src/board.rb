class Board
  attr_reader :grid

  def initialize
    @grid = reset
  end

  def reset
    @grid = [[0,0,0], [0,0,0], [0,0,0]]
  end

  def set(p, loc)
    @grid[loc[0]][loc[1]] = p if @grid[loc[0]][loc[1]] == 0
  end

  def winner?
    [:x, :o].each do |p|
      #horizontal/vertical wins
      [0,1,2].each do |n|
        return p if @grid[0][n] == p && @grid[1][n] == p && @grid[2][n] == p
        return p if @grid[n][0] == p && @grid[n][1] == p && @grid[n][2] == p
      end

      #diagonal lr + rl
      return p if @grid[0][0] == p && @grid[1][1] == p && @grid[2][2] == p
      return p if @grid[0][2] == p && @grid[1][1] == p && @grid[2][0] == p
    end

    nil
  end

  def print
    system "clear"
    @grid.each { |r| puts "[#{r[0]}][#{r[1]}][#{r[2]}]" }
  end
end
