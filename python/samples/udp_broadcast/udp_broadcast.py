# Send UDP broadcast packet

import socket

MYPORT = 50000 + 10

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('', 0))
sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
addr = ('<broadcast>', MYPORT)

sock.sendto('STE: serial number 12345678', addr)

sock.close()
