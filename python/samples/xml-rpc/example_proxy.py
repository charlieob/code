import xmlrpclib, httplib

class ProxiedTransport(xmlrpclib.Transport):
    def set_proxy(self, proxy):
        self.proxy = proxy
    def make_connection(self, host):
        self.realhost = host
        h = httplib.HTTP(self.proxy)
        return h
    def send_request(self, connection, handler, request_body):
        print("send_request: realhost=%s, handler=%s" % (self.realhost, handler))
        connection.putrequest("POST", 'http://%s%s' % (self.realhost, handler))
    def send_host(self, connection, host):
        connection.putheader('Host', self.realhost)

p = ProxiedTransport()
#p.set_proxy('proxy-server:8080')
p.set_proxy('w3proxy.s3group.com:3128')
server = xmlrpclib.ServerProxy('http://time.xmlrpc.com/RPC2', verbose=True, transport=p)
print server
currtime = server.currentTime.getCurrentTime()
print currtime
print currtime.value
