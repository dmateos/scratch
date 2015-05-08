require_relative("bundles")

class Product
  attr_reader :name, :code, :bundles

  def initialize(name, code, bundles = nil)
    @name = name
    @code = code
    @bundles = bundles
  end
end
