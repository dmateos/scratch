#!/usr/bin/ruby
require 'optparse'
require 'iptables'

# Parse the command args
optparse = OptionParser.new do |opts|
  opts.banner = ""

  opts.on("--start") do
    puts "start"
  end

  opts.on("--stop") do
    puts "stop"
  end

  opts.on("--restart") do
    puts "restart"
  end
end

optparse.parse

# Load all the rule classes in the rule dir.
ruleclasses = []
Dir.foreach("rules/") do |item|
  next if item == "." or item == ".."

  require "rules/#{item}"
  ruleclasses << Object.const_get(item.gsub(".rb", ""))
end

# Instance the rules
ruleclasses.each do |rule|
  testinstance = rule.new
  testinstance.check_syntax
end
