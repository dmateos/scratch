require 'spec_helper'
describe "Triangle" do
  let(:data_1) { [
      [3],
      [7,4],
      [2,4,6],
      [8,5,9,3]
    ]
  }

  let(:data_2) { [
      [75],
      [95,64],
      [17,47,82],
      [18,35,87,10],
      [20,04,82,47,65],
      [19,01,23,75,03,34],
      [88,02,77,73,07,63,67],
      [99,65,04,28,06,16,70,92],
      [41,41,26,56,83,40,80,70,33],
      [41,48,72,33,47,32,37,16,94,29],
      [53,71,44,65,25,43,91,52,97,51,14],
      [70,11,33,28,77,73,17,78,39,68,17,57],
      [91,71,52,38,17,14,91,43,58,50,27,29,48],
      [63,66,04,68,89,53,67,30,73,16,69,87,40,31],
      [04,62,98,27,23,9,70,98,73,93,38,53,60,04,23]
    ]
  }

  let(:large_triangle) { LargeTriangle.new(data_1) }
  let(:large_triangle_big) { LargeTriangle.new(data_2) }

  it "#best_path gets the best path for a triangle" do
    expect(large_triangle.best_path_element(0, 0)).to eq(7)
    expect(large_triangle.best_path_element(1, 0)).to eq(4)
    expect(large_triangle.best_path_element(2, 1)).to eq(9)
    expect(large_triangle.best_path_element(1, 1)).to eq(6)
  end

  it "#best_path_row gets the best path for a row" do
    expect(large_triangle.best_path_row(0)).to eq(7)
    expect(large_triangle.best_path_row(1)).to eq(6)
    expect(large_triangle.best_path_row(2)).to eq(9)
  end

  it "#best_path_brute_force stupidly find the best path" do
    expect(large_triangle.best_path_greedy).to eq(23)

    expect(large_triangle_big.best_path_greedy).to eq(1074)
  end
end
