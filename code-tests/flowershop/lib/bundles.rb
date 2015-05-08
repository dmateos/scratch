class Bundles
  def initialize
    new_bundles_list
  end

  def new_bundles_list
    @bundle_list = {}
  end

  def add_bundle(qty, price)
    @bundle_list[qty] = price
  end

  def get_by_qty(qty)
    @bundle_list[qty]
  end

  def get_optimal_bundles(qty)
    # Generate combinations of the availbile bundles starting from sets of 1 to see if we can find
    # the minimal bundles required for a given qty
    (1..qty).each do |q|
      @bundle_list.keys.repeated_combination(q).each do |perms|
        # we found one that matches
        if perms.inject(:+) == qty
          # build a structure (sorted by highest qty) out of the bundles 
          # ie [1,5,5] will be { 5 => [price, price], 1 => [price] }
          return perms.sort.reverse.each_with_object({}) do |p, hash|
            bundle_qty, bundle_price = p, @bundle_list[p]
            hash[bundle_qty] ||= []
            hash[bundle_qty] << bundle_price
          end
        end
      end
    end
    raise "could not find a suitable bundle"
  end
end
