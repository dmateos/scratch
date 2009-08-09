#! /usr/bin/python
import sys
import comprotocol

def main():
    print "Linguo *is* dead"

    # Jabber protocol, the protocol handler incase we want to batch some shit and some bindings. 
    pcontroller = comprotocol.Controller()
    pcontroller.add_protocol(comprotocol.Jabber("dmateosdev@gmail.com", "password"))
    pcontroller.add_protocol(comprotocol.MSN(None, None))

    # Message Echo, local print, and online status msging. 
    pcontroller.on_message(lambda con, send, msg: pcontroller.send_message(con, send, "you said %s, dumbass.." %(msg)))
    pcontroller.on_message(lambda con, send, msg: sys.stdout.write("%s said %s\n" %(send, msg)))
    pcontroller.on_userlog(lambda con, send: pcontroller.send_message(con, send, "hello i am skynet, would you like to play a game?"))

    while True:
        pcontroller.process()


if __name__ == "__main__":
    main()
