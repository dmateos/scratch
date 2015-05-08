class FakeStdin
  attr_reader :inputs

  def initialize
    @in = []
  end

  def <<(data)
    @in << data
  end

  def gets
    @in.shift
  end
end
