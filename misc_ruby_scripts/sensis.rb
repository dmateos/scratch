require 'rubygems'
require 'json'
require 'net/http'
require 'csv'
require 'pp'

def search(query = "", location = "")
  apikey = "4kp5sqjy4s3gkhsyspyarwdy"
  endpoint = "http://api.sensis.com.au/ob-20110511/test/search"
  url = "#{endpoint}?key=#{apikey}&query=#{URI.encode(query.to_s)}&location=#{URI.encode(location.to_s)}"

  response = Net::HTTP.get_response(URI.parse(url))
  response.error! unless response.instance_of? Net::HTTPOK
  result = JSON.parse(response.body)

  case result["code"]
  when 200
    return result
  when 206
    return result
  else
    raise "api error #{result["message"]}"
  end
end

count = 0
csv_out = CSV.open("output.csv", "a")
already_done_out = File.open("cache", "a")
already_done_data = File.read("cache").split("\n")

CSV.foreach("acnc.csv", encoding: "windows-1251:utf-8") do |row|
  if already_done_data.include?(row[0])
    puts "skipping #{row[0]}"
    next
  end

  puts "searching for #{row[1]}"
  sensis_data = search(row[1], row[7])
  data_array = [row[0], row[1]]

  begin
    row.each { |d| data_array << d }
    data_array << sensis_data["results"].first["name"]
    data_array << sensis_data["results"].first["primaryContacts"].first["value"]
  rescue
  end

  csv_out << data_array
  already_done_out << "#{row[0]}\n"
  count += 1
  puts "done #{count}"
  sleep 0.6
end

already_done_out.close
csv_out.close
