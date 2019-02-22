# see https://docs.python.org/3/library/argparse.html
# try this to test:
#   python argparse-sample.py -h
#   python argparse-sample.py -f foo -b bar -v file1 file2
#

import argparse, sys

def main():
    parser = argparse.ArgumentParser(description='Description of your program')
    parser.add_argument('-f','--foo', help='Description for foo argument', required=True)
    parser.add_argument('-b','--bar', help='Description for bar argument', required=True)
    parser.add_argument('-v', '--verbose', help='Option argument without a value', action='store_true')
    parser.add_argument('positional', nargs='+')
    args = vars(parser.parse_args())

    print(args)

if __name__ == '__main__':
    sys.exit(main())
