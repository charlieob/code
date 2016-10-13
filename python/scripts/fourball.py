import sys

def usage():
    print "usage: fourball.py <handicaps>"


def main(args):
    handicaps = sorted([int(a) for a in args])
    low = handicaps[0]
    adj = [((x-low)*3+2)/4 for x in handicaps if x > low]
    print " ".join(str(x) for x in adj)


if __name__=='__main__':

    if len(sys.argv) < 2:
        usage()
        sys.exit(2)

    main(sys.argv[1:])
