require_relative 'spec_helper'

describe "StringCalc" do
  let(:calc) { StringCalc.new }

  it "returns an empty string" do
    expect(calc.add("")).to eq(0) 
  end

  it "returns a single number" do
    expect(calc.add("1")).to eq(1)
  end

  it "returns the addition of any amount of numbers" do
    expect(calc.add("3,4,5,6")).to eq(18)
  end

  it "allows newlines between numbers" do
    expect(calc.add("1\n2,3")).to eq(6)
  end

  it "allows you to specify the delimiter format" do
    expect(calc.add("//;\n1;2")).to eq(3)
  end

  it "#delimiter_is? works out the delimiter" do
    expect(calc.delimiter_is?("//?\n")).to eq("?")
  end

  it "throws an exception on a negative number with numbers listed" do
    expect { calc.add("1,-2,-3") }.to raise_error(RuntimeError, "negative numbers are not allowed [-2, -3]")
  end

  it "#numbers_negative? finds negative number" do
    expect(calc.numbers_negative?([1,2,-3])).to eq([-3])
  end

  it "ignores numbers greater than 1k" do
    expect(calc.add("3,4,1000")).to eq(7)
  end

  it "#filter_size exludes numbers over 1k" do
    expect(calc.filter_size([2,3,1000])).to eq([2,3])
  end

  it "allows delimiters of any length between []'s" do
    expect(calc.add("//[***]\n1**2**3")).to eq(6)
  end
end
