class IPTablesInterface
  def initialize
    @binpath = "/sbin/iptables"
  end

  def exsists?
    system(@binpath + " -v")
  end

  def exec(command)
    system(@binpath + " " + command)
  end
end

class IPTablesCommand

end
