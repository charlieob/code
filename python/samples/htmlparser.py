from HTMLParser import HTMLParser
from urllib2 import urlopen
import sys
import pdb

class MyHTMLParser(HTMLParser):
    def __init__(self):
        HTMLParser.__init__(self)
        self.pieces = []

    def handle_data(self, text):
    # called for each block of plain text
        self.pieces.append(text)

    def output(self):
        """Return processed HTML as a single string"""
        pdb.set_trace()
        return "".join(self.pieces)

if __name__ == "__main__":
    for filename in sys.argv[1:]:
        source = open(filename)
        #sock = urlopen('http://www.diveintopython.org')
        htmlSource = source.read()
        source.close()
        parser = MyHTMLParser()
        parser.feed(htmlSource)
        parser.close()
        print parser.output()
