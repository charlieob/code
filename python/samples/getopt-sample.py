# https://docs.python.org/3/library/getopt.html
# The getopt module is a parser for command line options whose API is designed to be familiar
# to users of the C getopt() function. Users who are unfamiliar with the C getopt() function
# or who would like to write less code and get better help and error messages should consider
# using the argparse module instead.

import getopt, sys

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "ho:v", ["help", "output="])
    except getopt.GetoptError as err:
        # print help information and exit:
        print str(err)  # will print something like "option -a not recognized"
        usage()
        sys.exit(2)
    output = None
    verbose = False
    for opt, arg in opts:
        if opt == "-v":
            verbose = True
        elif opt in ("-h", "--help"):
            usage()
            sys.exit()
        elif opt in ("-o", "--output"):
            output = arg
        else:
            assert False, "unhandled option"
    # ...

if __name__ == "__main__":
    main()
