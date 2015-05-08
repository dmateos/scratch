require_relative("bundles")
require_relative("product")

class FlowerShop
  attr_accessor :products

  def initialize
    @products = []
  end

  def add_products(products)
    # given a formated hash of products and bundles, generate
    # the bundle objects and add them to new product objects.
    products.each do |prod_name, data|
      bundles = Bundles.new
      data[:bundles].each do |bundle_qty, bundle_price|
        bundles.add_bundle(bundle_qty, bundle_price)
      end
      @products << Product.new(prod_name, data[:id], bundles)
    end
  end

  def get_product_by_code(code)
    @products.select { |p| p.code == code }.first
  end

  def print_best_bundles_for_code(code, qty, to_stdout=true)
    print_best_bundles(get_product_by_code(code), qty, to_stdout)
  end

  private
  def print_best_bundles(product, qty, to_stdout=true)
    output_str = ""

    begin
      bundles = product.bundles.get_optimal_bundles(qty)
      bundle_total_price = bundles.map { |bundle_qty, prices| prices }.flatten.inject(:+)

      output_str += "#{qty} #{product.code} $#{bundle_total_price.round(3)}\n"

      bundles.each do |bundle_qty, prices|
        output_str += "\t#{prices.count} x #{bundle_qty} $#{prices.first}\n"
      end
    rescue
      output_str += "could not find a bundle" 
    end

    return output_str if not to_stdout
    puts output_str
  end
end

