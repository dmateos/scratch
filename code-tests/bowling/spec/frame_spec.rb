require 'spec_helper'

describe "Frame" do
  before :each do
    @frame = Frame.new(1)
  end

  it "creates a new game" do
    expect(@frame).to be_a(Frame)
  end

  it "registers and adds two scores" do
    @frame.add_score(5)
    @frame.add_score(2)
    expect(@frame.score).to eq(7)
  end

  it "checks if a frame is finished by taking two turns" do
    @frame.add_score(5)
    expect(@frame.finished?).to be false
    @frame.add_score(4)
    expect(@frame.finished?).to be true
  end

  it "checks if a frame is finished by a strike" do
    @frame.add_score(10)
    expect(@frame.finished?).to be true
  end

  it "detects a strike" do
    expect(@frame.strike?).to be false
    @frame.add_score(10)
    expect(@frame.strike?).to be true
  end

  it "detects a spare" do
    @frame.add_score(5)
    expect(@frame.spare?).to be false

    @frame.add_score(5)
    expect(@frame.spare?).to be true
  end
end
