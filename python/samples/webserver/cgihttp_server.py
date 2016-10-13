import BaseHTTPServer
import SimpleHTTPServer
import CGIHTTPServer

def run_it(HandlerClass = CGIHTTPServer.CGIHTTPRequestHandler, ServerClass = BaseHTTPServer.HTTPServer):
    SimpleHTTPServer.test(HandlerClass, ServerClass)

try:
    print 'Use Control-C to exit'
    run_it()
except KeyboardInterrupt:
    print 'Exiting'
