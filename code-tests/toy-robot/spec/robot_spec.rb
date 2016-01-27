require 'spec_helper'

describe "Robot" do
  let(:table) { Table.new(4, 4) }

  describe "has been placed" do
    let(:robot) { Robot.new(table) }

    before do
      robot.place("0,0,NORTH")
    end

    it "gets placed in a specific spot" do
      robot.place("4,4,WEST")
      expect(robot.position).to eq(Position.new(4,4,:west))
    end

    it "gets placed multiple times in a row" do
      robot.place("4,4,WEST")
      robot.place("3,3,NORTH")
      expect(robot.position).to eq(Position.new(3,3,:north))
    end

    it "wont place on the outside of the table" do
      robot.place("5,4,WEST")
      expect(robot.position).to eq(Position.new(0,0,:north))
    end

    it "moves" do
      robot.move
      expect(robot.position.y).to eq(1)
    end

    it "turns left" do
      robot.left
      expect(robot.position.orientation).to eq(:west)
    end

    it "turns right" do
      robot.right
      expect(robot.position.orientation).to eq(:east)
    end

    it "reports status" do
      string = robot.report(false)
      expect(string).to eq("0,0,NORTH")
    end

    it "stops when at edge of table" do
      10.times do
        robot.move
      end
      expect(robot.position.y).to eq(4)
    end
  end

  describe "hasnt been placed yet" do
    let(:robot) { Robot.new(table) }

    it "doesn't move" do
      robot.move
      expect(robot.position.y).to eq(nil)
    end

    it "doesn't turn left" do
      robot.left
      expect(robot.position.orientation).to eq(nil)
    end

    it "doesn't turn right" do
      robot.left
      expect(robot.position.orientation).to eq(nil)
    end

    it "doesn't report status" do
      str = robot.report
      expect(robot.report).to eq(nil)
    end
  end
end
