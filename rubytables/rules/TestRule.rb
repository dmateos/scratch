require "rule"

class TestRule < Rule
  def initialize
    puts "I am TestRule"
  end

  raw_rule "iptables -A FWD WHATEVER"
end
