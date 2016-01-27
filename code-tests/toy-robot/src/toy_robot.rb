require_relative 'robot'
require_relative 'command_reader'

class ToyRobot
  attr_reader :robot

  def initialize(options = {})
    x = options.fetch(:x, 4)
    y = options.fetch(:y, 4)
    stdin = options.fetch(:stdin, $stdin)

    @commands = CommandReader.new(stdin)
    @table = Table.new(x, y)
    @robot = Robot.new(@table)
  end

  def run
    @commands.each do |op, args|
      next if op.nil?
      op_sym = op.downcase.to_sym
      robot.send(op_sym, *args) if robot.respond_to?(op_sym)
    end
  end
end
