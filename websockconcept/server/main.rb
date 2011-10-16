#! /usr/bin/ruby

require 'rubygems'
require 'em-websocket'

class Session
  attr_reader :socket
  attr_accessor :name

  def initialize(socket)
    @socket = socket
    @name = nil
  end
end

Host = "0.0.0.0"
Port = 8080
users = {}

EventMachine::WebSocket.start(:host => Host, :port => Port) do |ws|
  puts "new connection"

  ws.onopen do
    users[ws] = Session.new(ws)
    puts "initialized"
  end

  ws.onclose do
    puts "exit"
  end

  ws.onmessage do |msg|
    user = users[ws]
    puts "unauthed request" if user.name.nil? and msg.text.contains("name: ")
  end
end
