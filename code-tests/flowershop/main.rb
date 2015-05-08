#! /usr/bin/env ruby
require_relative("lib/flowershop")

PRODUCTS = {
  "Roses" => { id: "R12", bundles:  { 5 => 6.99 , 10 => 12.99 } },
  "Lilies" => { id: "L09", bundles: { 3 => 9.95, 6 => 16.95, 9 => 24.95 } },
  "Tulips" => { id: "T58", bundles: { 3 => 5.95, 5 => 9.95, 9 => 16.99 } }
}

flowershop = FlowerShop.new
flowershop.add_products(PRODUCTS)

loop do
  qty, code = gets.strip.split
  flowershop.print_best_bundles_for_code(code.upcase, qty.to_i) if qty.to_i != 0 and not code.nil?
end
