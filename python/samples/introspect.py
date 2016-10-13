import sys

print dir(sys.stdout)
print sys.stdout.encoding
print getattr(sys.stdout, "encoding")
# in a DOS cmd window, the encoding is 'cp437'
# look up 'ascii' on wikipedia and look for the details of encoding schemes at the bottom

print sys.stdout.__doc__
help(sys.stdout)
# that gives the details of a file object, which is what stdout is
