# listen for a UDP broadcast

import socket

MYPORT = 50000 + 10

# Set the socket parameters
addr = ('', MYPORT)  # host, port

# Create socket and bind to address
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(addr)

# Receive message
data, addr = sock.recvfrom(1024)
print "From addr: '%s', msg: '%s'" % (addr[0], data)

# Close socket
sock.close()
