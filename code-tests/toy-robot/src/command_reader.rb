class CommandReader
  attr_reader :input

  def initialize(data = $stdin)
    @input = data
  end

  def each
    while cmd = @input.gets
      op, args = parse_command(cmd)
      yield op, args
    end
  end

  private
  def parse_command(command)
    op, args = command.split(/\s+/)
  end
end
