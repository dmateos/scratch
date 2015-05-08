require "spec_helper"

describe "Board" do
  describe "initialize board" do
    let(:board) { Board.new }

    it "creates a new board object" do
      expect(board).to be_a(Board)
    end

    it "defines a 3x3 grid" do
      expect(board.grid.flatten.length).to eq(9)
    end
  end

  describe "make move on board" do
    let(:board) { Board.new }

    it "marks an x on the board" do
      board.set(:x, [1,1])
      expect(board.grid[1][1]).to eq(:x)
    end

    it "marks a o on the board" do
      board.set(:o, [1,2])
      expect(board.grid[1][2]).to eq(:o)
    end

    it "does not allow double moves" do
      expect(board.set(:x, [1,1])).to be(:x)
      expect(board.set(:o, [1,1])).to be(nil)
      expect(board.grid[1][1]).to eq(:x)
    end
  end

  describe "resets the boards state" do
    let(:board) { Board.new }

    it "resets all positions backt to 0" do
      board.set(:x, [1,1])
      board.reset
      expect(board.grid[1][1]).to eq(0)
    end
  end

  describe "checks if there is a winner" do
    let(:board) { Board.new }

    it "detects a winner from vertical" do
      [:x, :o].each do |p|
        [0,1,2].each do |n|
          board.reset
          board.set(p, [0, n])
          board.set(p, [1, n])
          board.set(p, [2, n])
          expect(board.winner?).to eq(p)
        end
      end
    end

    it "detects a winner from horizontal" do
      [:x, :o].each do |p|
        [0,1,2].each do |n|
          board.reset
          board.set(p, [n, 0])
          board.set(p, [n, 1])
          board.set(p, [n, 2])
          expect(board.winner?).to eq(p)
        end
      end
    end

    it "detects a LR diagonal winner" do
      [:x, :o].each do |p|
        board.reset
        board.set(p, [0,0])
        board.set(p, [1,1])
        board.set(p, [2,2])
        expect(board.winner?).to eq(p)
      end
    end

    it "detects a RL diagonal winner" do
      [:x, :o].each do |p|
        board.reset
        board.set(p, [0,2])
        board.set(p, [1,1])
        board.set(p, [2,0])
        expect(board.winner?).to eq(p)
      end
    end
  end
end
