require_relative("spec_helper")

describe "product" do
  let(:product) { Product.new("Roses", "R12") }

  it "returns the correct name and model" do
    expect(product.name).to eq("Roses")
    expect(product.code).to eq("R12")
  end
end
