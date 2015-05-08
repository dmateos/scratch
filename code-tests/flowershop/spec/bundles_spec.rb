require_relative("spec_helper")

describe "bundle" do
  let(:bundles) { Bundles.new }

  it "retrieves the specified bundle for the given qty" do
    bundles.add_bundle(99, 9.99)
    expect(bundles.get_by_qty(99)).to eq(9.99)
  end

  it "gets the optimal set of bundles for the given qty" do
    bundles.add_bundle(3, 5.95)
    bundles.add_bundle(5, 9.95)
    bundles.add_bundle(9, 16.99)
    expect(bundles.get_optimal_bundles(13)).to eq({ 5 => [9.95, 9.95], 3 => [5.95]})

    bundles.new_bundles_list
    bundles.add_bundle(5, 6.99)
    bundles.add_bundle(10, 12.99)
    expect(bundles.get_optimal_bundles(10)).to eq({10 => [12.99]})

    bundles.new_bundles_list
    bundles.add_bundle(3, 9.95)
    bundles.add_bundle(6, 16.95)
    bundles.add_bundle(9, 24.95)
    expect(bundles.get_optimal_bundles(15)).to eq({ 9 => [24.95], 6 => [16.95]})
  end

  it "raises an exception if a suitable bundle can't be found" do
    bundles.new_bundles_list
    bundles.add_bundle(10, 9.95)
    expect {bundles.get_optimal_bundles(15)}.to raise_error("could not find a suitable bundle")
  end
end
