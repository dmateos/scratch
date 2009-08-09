import xmpp

class Controller:
    """Controller for the various protocols, if they follow the template."""

    def __init__(self):
        self.protocols = []    

        # Callback dict and associated sublists of callies.
        self.callbacks = dict()
        self.callbacks["messages"] = []
        self.callbacks["userlog"] = []

        print("ProtocolController: UP")
 
    def add_protocol(self, protocol):
        """Register a protocol with the controller class."""
        self.protocols.append(protocol)
        protocol.on_message(self._message_cb)
        protocol.on_userlog(self._userlog_cb)
        
        if protocol.connect():
            print("ProtocolController: Init %s OK" %(protocol.PID))
            return True
        else:
            print("ProtocolController: Init %s FAIL" %(protocol.PID))
            return False

    def process(self):
        """Batch proc all protocols."""
        for prot in self.protocols:
            prot.process()

    def send_message(self, con, to, message):
        print("ProtocolController: send_message to %s" %(to))
        con.send_message(to, message)

    def on_message(self, callback):
        """Batch subscribe to protocol messages."""
        print("ProtocolController: on_message append %s" %(callback))
        self.callbacks["messages"].append(callback)

    def _message_cb(self, connection, sender, message):
        """Internal to catch protocol msg callbacks and batch em."""
        for callback in self.callbacks["messages"]:
            print("ProtocolController: message CB from %s" %(sender))
            callback(connection, sender, message)

    def on_userlog(self, callback):
        self.callbacks["userlog"].append(callback)
        print("ProtocolController: on_userlog append %s" %(callback))

    def _userlog_cb(self, connection, sender):
        for callback in self.callbacks["userlog"]:
            print("ProtocolController: userlog CB from %s" %(sender))
            callback(connection, sender)


class Jabber:
    """Jabber protocol lib wrapper to work in a ProtocolController."""
    
    PID = "JABBER"

    def __init__(self, user, password, server=None):
        # Setup some user info 
        jid = xmpp.JID(user)
        self.user = jid.getNode()
        self.password = password
        self.callbacks = dict()

        if server == None:
            self.server = jid.getDomain()
        else: 
            self.server = server

    def connect(self):
        """Establish a connection and auth to server."""
        self.connection = xmpp.Client(self.server, debug=[])

        # Connect, auth and bail if we fail. (haw)
        if not self.connection.connect():
            return False
        elif not self.connection.auth(self.user, self.password, "snakeman"):
            return False

        #TODO: Lambda trick here rather than actual methods?
        self.connection.RegisterHandler("message", self._message_cb)
        self.connection.RegisterHandler("presence", self._presence_cb)
        self.connection.sendInitPresence()

        self.send_message("dmateos@gmail.com", "Linguo *is* dead.")
        return True

    def process(self):
        """Proccess some server events."""
        self.connection.Process(1)

    def send_message(self, to, message):
        """Send a message to the specified recipient."""
        self.connection.send(xmpp.Message(to, message))       

    def on_message(self, callback):
        """Register a message event handler."""
        self.callbacks["message"] = callback
 
    def _message_cb(self, connection, message):
        """Internal message callback handler."""
        self.callbacks["message"](self, message.getFrom(), message.getBody())

    def on_userlog(self, callback):
        self.callbacks["userlog"] = callback

    def _presence_cb(self, connection, pres):
        self.callbacks["userlog"](self, pres.getFrom())


class MSN:
    PID = "MSN"

    def __init__(self, user, password):
        pass

    def connect(self):
        pass

    def process(self):
        pass

    def send_message(self, to, message):
        pass

    def on_message(self, callback):
        pass

    def _message_cb(self, connection, message):
        pass

    def on_userlog(self, callback):
        pass

    def _userlog_cb(self, connection, event):
        pass
