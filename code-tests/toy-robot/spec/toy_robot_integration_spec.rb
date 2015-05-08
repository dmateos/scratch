require_relative 'spec_helper'

describe "ToyRobot Integration" do
  before :each do
    @stdin = FakeStdin.new
    @toy_robot = ToyRobot.new(stdin: @stdin)
  end

  it "runs example a in the PDF" do
    @stdin << "PLACE 0,0,NORTH" << "MOVE" << "REPORT"
    @toy_robot.run
    expect(@toy_robot.robot.report(false)).to eq("0,1,NORTH")
  end

  it "runs example b in the PDF" do
    @stdin << "PLACE 0,0,NORTH" << "LEFT" << "REPORT"
    @toy_robot.run
    expect(@toy_robot.robot.report(false)).to eq("0,0,WEST")
  end

  it "runs example c in the PDF" do
    @stdin << "PLACE 1,2,EAST" << "MOVE" << "MOVE" << "LEFT" << "MOVE" << "REPORT"
    @toy_robot.run
    expect(@toy_robot.robot.report(false)).to eq("3,3,NORTH")
  end

  it "wont place over the edge of the table" do
    @stdin << "PLACE 5,5,NORTH"
    @toy_robot.run
    expect(@toy_robot.robot.position).to_not eq(Position.new(5,5,:north))
  end

  it "ignores place with invalid orientation" do
    @stdin << "PLACE 0,0,NOTHR" << "RIGHT" << "MOVE"
    @toy_robot.run
    expect(@toy_robot.robot.position).to be_a(NilPosition)
  end

  it "will place multiple times" do
    @stdin << "PLACE 0,0,NORTH" << "PLACE 1,1,SOUTH"
    @toy_robot.run
    expect(@toy_robot.robot.position).to eq(Position.new(1, 1, :south))
  end

  it "will place then ignore invalid place" do
    @stdin << "PLACE 0,0,NORTH" << "PLACE 0,1,NOTHR" << "MOVE"
    @toy_robot.run
    expect(@toy_robot.robot.position).to eq(Position.new(0, 1, :north))
  end

  it "wont go over edge of table" do
    @stdin << "PLACE 0,3,NORTH" << "MOVE" << "MOVE"
    @toy_robot.run
    expect(@toy_robot.robot.position).to eq(Position.new(0, 4, :north))
  end

  it "turns and moves north" do
    @stdin << "PLACE 0,0,EAST" << "LEFT" << "MOVE"
    @toy_robot.run
    expect(@toy_robot.robot.position).to eq(Position.new(0, 1, :north))
  end

  it "turns and moves east" do
    @stdin << "PLACE 0,0,NORTH" << "RIGHT" << "MOVE"
    @toy_robot.run
    expect(@toy_robot.robot.position).to eq(Position.new(1, 0, :east))
  end

  it "turns and moves south" do
    @stdin << "PLACE 0,1,WEST" << "LEFT" << "MOVE"
    @toy_robot.run
    expect(@toy_robot.robot.position).to eq(Position.new(0, 0, :south))
  end

  it "turns and moves west" do
    @stdin << "PLACE 1,0,SOUTH" << "RIGHT" << "MOVE"
    @toy_robot.run
    expect(@toy_robot.robot.position).to eq(Position.new(0, 0, :west))
  end

  describe "collision with placed object" do
    before :each do
      @stdin = FakeStdin.new
      @toy_robot = ToyRobot.new(stdin: @stdin)
      @stdin << "PLACE 2,2,NORTH"
    end

    it "doesnt collide with an object to its north" do
      @stdin << "PLACE_OBJECT" << "MOVE"
      @toy_robot.run
      expect(@toy_robot.robot.position).to eq(Position.new(2, 2, :north))
    end

    it "doesnt collide with an object to its south" do
      @stdin << "RIGHT" << "RIGHT" << "PLACE_OBJECT" << "MOVE"
      @toy_robot.run
      expect(@toy_robot.robot.position).to eq(Position.new(2, 2, :south))
    end

    it "doesnt collide with an object to its east" do
      @stdin << "RIGHT" << "PLACE_OBJECT" << "MOVE"
      @toy_robot.run
      expect(@toy_robot.robot.position).to eq(Position.new(2, 2, :east))
    end

    it "doesnt collide with an object to its west" do
      @stdin << "LEFT" << "PLACE_OBJECT" << "MOVE"
      @toy_robot.run
      expect(@toy_robot.robot.position).to eq(Position.new(2, 2, :west))
    end
  end
end
