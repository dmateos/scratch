class LargeTriangle
  def initialize(array)
    @array = array
  end

  def best_path_element(depth, element)
    n1 = @array[depth+1][element]
    n2 = @array[depth+1][element+1]
    n1 > n2 ? n1 : n2
  end

  def best_path_row(depth) 
    @array[depth].length.times.map { |e| best_path_element(depth, e) }.max
  end

  def best_path_greedy(path = [], d = 0, e = 0)
    path << @array[d][e] if path.empty?

    if d != @array.length-1
      path << best_path_element(d, e)
      best_path_greedy(path, d+1, @array[d+1].index(path.last))
    else
      path.inject(0, &:+)
    end
  end
end
