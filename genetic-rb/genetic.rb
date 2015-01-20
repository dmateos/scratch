require 'similar_text'

STR = 'legendary'
POOLSIZE = 3000
MUTATION_RATE = (0..250)
SLEEP = 0.1

class Chromosome
  attr_accessor :base, :fitness, :generation

  def initialize(arg = nil, gen = 0)
    #base is a random string unless another is supplied
    @base = (arg.nil? ? (0..STR.length-1).map { (' '..'z').to_a.sample }.join : arg)
    @fitness = @base.similar(STR)
    @generation = gen
  end

  def mutate()
    # 1/MUTATION_RATE chance to replace each char in the base with a new random char.
    @base = @base.each_char.map { |char| rand(MUTATION_RATE) == 0 ? (' '..'z').to_a.sample : char }.join
    self
  end

  def self.child(parent_one, parent_two)
    #Make a child whos base string is a combination of two parents
    crosspoint = parent_one.base.length/2
    Chromosome.new(parent_one.base[0..crosspoint-1] + parent_two.base[crosspoint..parent_two.base.length], parent_one.generation+1)
  end
end

pool = (0..POOLSIZE).to_a.map { Chromosome.new }

loop do
  average_fitness = pool.inject(0) { |result, c| result + c.fitness } / pool.length

  puts "\e[H\e[2J"
  puts "average fitness = #{average_fitness}"
  puts "----"
  pool.each { |c|
    puts "#{c.base} rating:#{c.fitness} generation:#{c.generation}"
  }
  #check if we have a winner
  pool.select { |c| c.fitness == 100 }.length > 0 ? exit : nil

  # nope so select some parents and make a new pool of children
  # atm were being quite eliteist
  parents = pool.select { |c| c.fitness >= average_fitness-0.0001 }

  pool = (0..POOLSIZE).to_a.map { Chromosome.child(parents.sample, parents.sample).mutate }

  sleep(SLEEP) if not SLEEP.nil?
end
