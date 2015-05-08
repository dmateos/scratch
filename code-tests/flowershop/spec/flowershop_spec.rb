require_relative("spec_helper")

describe "flowershop" do
  let(:flowershop) { FlowerShop.new }

  let(:test_products) { {
    "Roses" => { id: "R12", bundles:  { 5 => 6.99 , 10 => 12.99 } },
    "Lilies" => { id: "L09", bundles: { 3 => 9.95, 6 => 16.95, 9 => 24.95 } },
    "Tulips" => { id: "T58", bundles: { 3 => 5.95, 5 => 9.95, 9 => 16.99 } }
  } }

  it "parses and adds a list of products with bundles" do
    flowershop.add_products(test_products) 
    expect(flowershop.products.count).to eq(3)
  end

  it "can retrieve product by code" do
    flowershop.add_products(test_products) 
    expect(flowershop.products.first.name).to eq("Roses")
    expect(flowershop.products.first.code).to eq("R12")
  end

  it "gets the correct output for the test cases defined by the spec file" do
    flowershop.add_products(test_products)
    expect(flowershop.print_best_bundles_for_code("R12", 10, false)).to eq("10 R12 $12.99\n\t1 x 10 $12.99\n")
    expect(flowershop.print_best_bundles_for_code("L09", 15, false)).to eq("15 L09 $41.9\n\t1 x 9 $24.95\n\t1 x 6 $16.95\n")
    expect(flowershop.print_best_bundles_for_code("T58", 13, false)).to eq("13 T58 $25.85\n\t2 x 5 $9.95\n\t1 x 3 $5.95\n")
  end
end
