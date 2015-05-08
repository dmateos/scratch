require_relative 'robot'
require_relative 'command_reader'
require_relative 'placed_object'

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

      if @robot.respond_to?(op_sym)
        @robot.send(op_sym, *args)
      elsif op_sym == :place_object
        @table.place_object(PlacedObject.new(@robot.position))
      elsif op_sym == :map
        puts @table.map(@robot) + "\n"
      end

      draw
    end
  end

  def draw
    system "clear"
    puts @table.map(@robot)
    sleep 0.5
  end
end
