require 'rubygems'
require 'em-websocket'

host = "0.0.0.0"
port = 8080
clients = {}

EventMachine::WebSocket.start(:host => host, :port => port) do |socket|
  puts "new connection"

  socket.onopen do
    socket.send "hello client"
    clients[socket.object_id] = socket
  
    clients.each do |key, c|
      c.send "#{socket.object_id} has connected"
    end
  end

  socket.onmessage do |msg|
    clients.each do |key, c| 
      c.send "#{socket.object_id}: #{msg}"
    end
  end

  socket.onclose do
    puts "closed connection"
    clients.delete(socket.object_id)
    
    clients.each do |key, c|
      c.send "#{socket.object_id} has disconnected"
    end
  end

  socket.onerror do
    puts "error"
  end
end
