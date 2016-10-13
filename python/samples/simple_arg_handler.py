import sys
import os

def usage():
    print "usage: {} <arg>".format(os.path.basename(sys.argv[0]))

def main(args):
    print(args[0])

if __name__=='__main__':

    if len(sys.argv) != 2:
        usage()
        sys.exit(2)

    main(sys.argv[1:])
