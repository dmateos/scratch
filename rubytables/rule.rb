class Rule
  def self.raw_rule(arg)
    self.class.instance_variable_set(:@raw_rule, arg)
  end

  def check_syntax
    puts @raw_rule if not @raw_rule.nil?
    puts "syntax error for #{self.class.name}"
  end
end
