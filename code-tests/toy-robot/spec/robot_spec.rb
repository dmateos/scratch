require 'spec_helper'

describe "Robot" do
  describe "has been placed" do
    before do
      @table = Table.new(4,4)
      @robot = Robot.new(@table)
      @robot.place("0,0,NORTH")
    end

    it "gets placed in a specific spot" do
      @robot.place("4,4,WEST")
      expect(@robot.position).to eq(Position.new(4,4,:west))
    end

    it "gets placed multiple times in a row" do
      @robot.place("4,4,WEST")
      @robot.place("3,3,NORTH")
      expect(@robot.position).to eq(Position.new(3,3,:north))
    end

    it "wont place on the outside of the table" do
      @robot.place("5,4,WEST")
      expect(@robot.position).to eq(Position.new(0,0,:north))
    end

    it "moves" do
      @robot.move
      expect(@robot.position.y).to eq(1)
    end

    it "turns left" do
      @robot.left
      expect(@robot.position.orientation).to eq(:west)
    end

    it "turns right" do
      @robot.right
      expect(@robot.position.orientation).to eq(:east)
    end

    it "reports status" do
      string = @robot.report(false)
      expect(string).to eq("0,0,NORTH")
    end

    it "stops when at edge of table" do
      10.times do 
        @robot.move
      end
      expect(@robot.position.y).to eq(4)
    end
  end

  describe "doesnt run into objects placed in its path" do
    before do
      @table = Table.new(4,4)
      @robot = Robot.new(@table)
      @robot.place("2,2,NORTH")
    end

    it "doesnt run into an object to its north" do
      @table.place_object(PlacedObject.new(Position.new(2, 3, nil)))
      @robot.move
      expect(@robot.position).to eq(Position.new(2, 2, :north))
    end

    it "doesnt run into an object to its south" do
      @table.place_object(PlacedObject.new(Position.new(2, 1, nil)))
      @robot.right
      @robot.right
      @robot.move
      expect(@robot.position).to eq(Position.new(2, 2, :south))
    end

    it "doesnt run into an objec to its east" do
      @table.place_object(PlacedObject.new(Position.new(3, 2, nil)))
      @robot.right
      @robot.move
      expect(@robot.position).to eq(Position.new(2, 2, :east))
    end

    it "doesnt run into an object to its west" do
      @table.place_object(PlacedObject.new(Position.new(1, 2, nil)))
      @robot.left
      @robot.move
      expect(@robot.position).to eq(Position.new(2, 2, :west))
    end
  end

  describe "hasnt been placed yet" do
    before do
      @table = Table.new(4,4)
      @robot = Robot.new(@table)
    end

    it "doesn't move" do
      @robot.move
      expect(@robot.position.y).to eq(nil)
    end

    it "doesn't turn left" do
      @robot.left
      expect(@robot.position.orientation).to eq(nil)
    end

    it "doesn't turn right" do
      @robot.left
      expect(@robot.position.orientation).to eq(nil)
    end

    it "doesn't report status" do
      str = @robot.report
      expect(@robot.report).to eq(nil)
    end
  end
end
