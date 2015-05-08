class Frame
  attr_reader :frame_number

  def initialize(frame)
    @tries = []
    @pins = 10
    @frame_number = frame
  end

  def add_score(number)
    if !finished? && number <= @pins
      @tries << number
      @pins -= number
    end
  end

  def score
    @tries.inject(0, :+)
  end

  def finished?
    @tries.count >= 2 || @pins == 0
  end

  def strike?
    @tries.first == 10
  end

  def spare?
    !strike? && @pins == 0
  end
end

