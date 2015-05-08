require_relative 'spec_helper'

describe "Position" do
  before do
    @position = Position.new(10, 10, :north)
  end

  it "sets the x value on creation" do
    expect(@position.x).to eq(10)
  end

  it "sets the y value on creation" do
    expect(@position.y).to eq(10)
  end

  it "sets the orientation value" do
    expect(@position.orientation).to eq(:north)
  end

  it "doesn't set an invalid orientation" do
    @position = Position.new(10, 10, :norht)
    expect(@position.orientation).to eq(nil)
  end

  it "compares correct with another identical position object" do
    position2 = Position.new(10, 10, :north)
    expect(@position).to eq(position2)
  end

  it "compares not correct with a different position object" do
    position2 = Position.new(10, 9, :north)
    expect(@position).to_not eq(position2)
  end

  it "turns left" do
    @position.turn_left
    expect(@position.orientation).to eq(:west)
  end

  it "turns right" do
    @position.turn_right
    expect(@position.orientation).to eq(:east)
  end

  it "moves north" do
    @position.move
    expect(@position).to eq(Position.new(10, 11, :north))
  end

  it "moves east" do
    @position.turn_right
    @position.move
    expect(@position).to eq(Position.new(11, 10, :east))
  end 

  it "moves south" do 
    @position.turn_right
    @position.turn_right
    @position.move
    expect(@position).to eq(Position.new(10, 9, :south))
  end

  it "moves west" do
    @position.turn_left
    @position.move
    expect(@position).to eq(Position.new(9, 10, :west))
  end

  it "returns a valid string representation" do
    expect(@position.to_s).to eq("10,10,NORTH")
  end
end
