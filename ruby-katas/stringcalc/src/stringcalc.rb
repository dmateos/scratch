class StringCalc
  def add(numbers)
    delim = delimiter_is?(numbers)
    numbers = split_numbers(numbers, delim).map(&:to_i)

    if numbers_negative?(numbers).empty?
      filter_size(numbers).inject(0, :+)
    else
      raise "negative numbers are not allowed #{numbers_negative?(numbers).to_s}"
    end
  end

  def split_numbers(numbers, delims)
    numbers.split(/[\s,\n,#{delims}]/)
  end

  def numbers_negative?(numbers)
    numbers.select { |n| n < 0 }
  end

  def delimiter_is?(numbers)
    numbers[2] if numbers.start_with?("//")
  end

  def filter_size(numbers)
    numbers.select { |n| n < 1000 }
  end
end
