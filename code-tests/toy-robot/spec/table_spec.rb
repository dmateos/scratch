require_relative 'spec_helper'

describe "Table" do
  before do
    @table = Table.new(4, 4)
  end

  it "sets table maximum x value" do
    expect(@table.xmax).to eq(4)
  end

  it "sets table maxiumum y value" do
    expect(@table.ymax).to eq(4)
  end

  it "detects if a position is on the tables surface" do
    expect(@table.still_on_surface?(Position.new(2, 2, :north))).to eq(true)
  end

  it "detects if a position is outside of the tables surface" do
    expect(@table.still_on_surface?(Position.new(5, 4, :north))).to eq(false)
    expect(@table.still_on_surface?(Position.new(0, -1, :north))).to eq(false)
  end

  describe "placed objects" do
    before do
      @table = Table.new(4,4)
    end

    it "places an object on the table" do
      @table.place_object(PlacedObject.new(Position.new(2, 2, nil)))
      expect(@table.placed_objects.count).to eq(1)
    end

    it "checks if an object has collided with a given position" do
      @table.place_object(PlacedObject.new(Position.new(2, 2, nil)))
      expect(@table.collide_with_object?(Position.new(2,2,nil))).to eq(true)
    end

    it "checks if an object hasn't collided with a given position" do
      @table.place_object(PlacedObject.new(Position.new(2, 2, nil)))
      expect(@table.collide_with_object?(Position.new(1,2,nil))).to eq(false)
    end
  end
end
