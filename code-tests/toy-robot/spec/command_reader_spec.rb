require 'spec_helper'

describe "CommandReader" do
  before :each do
    @stdin = FakeStdin.new
    @command_reader = CommandReader.new(@stdin)
  end

  it "accepts new commands via stdin compadible interface" do
    @stdin << "MOVE" << "MOVE"
    expect(@command_reader.input).to eq(@stdin)
  end

  it "iterates over each command" do
    @stdin << "MOVE" << "MOVE"
    @command_reader.each do |op|
      expect(op).to eq("MOVE")
    end
  end

  it "parses a command into op and argument" do
    @stdin << "PLACE 0,0,NORTH"
    @command_reader.each do |op, args|
      expect(op).to eq("PLACE")
      expect(args).to eq("0,0,NORTH")
    end
  end
end
