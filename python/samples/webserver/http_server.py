import SimpleHTTPServer
import SocketServer

PORT = 8000

Handler = SimpleHTTPServer.SimpleHTTPRequestHandler

httpd = SocketServer.TCPServer(("", PORT), Handler)

print "serving at port", PORT
try:
    print 'Use Control-C to exit'
    httpd.serve_forever()
except KeyboardInterrupt:
    print 'Exiting'

