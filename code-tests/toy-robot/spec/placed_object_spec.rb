require_relative 'spec_helper'

describe "PlacedObject" do
  before :each do
    @position = Position.new(2, 2, nil)
    @placed_object = PlacedObject.new(@position)
  end

  it "gets assigned a cloned position in constructor" do
    expect(@placed_object.position).to eq(@position)
    expect(@placed_object.position).to_not equal(@position)
  end

  it "compares correct to another same position" do
    expect(@placed_object.position).to eq(Position.new(2, 2, nil))
  end

  it "compares incorrect to a different position" do
    expect(@placed_object.position).to_not eq(Position.new(2, 3, nil))
  end
end
