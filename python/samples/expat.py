import sys
import xml.parsers.expat

# 3 handler functions
def start_element(name, attrs):
    sys.stdout.write('<%s>' % name)
    #print 'Start element:', name, attrs
def end_element(name):
    sys.stdout.write('</%s>' % name)
    #print 'End element:', name
def char_data(data):
    sys.stdout.write('%s' % data)
    #print 'Character data:', repr(data)
    
def test(inFileName):
    fsock = open(inFileName, "r")
    
    p = xml.parsers.expat.ParserCreate()

    p.StartElementHandler = start_element
    p.EndElementHandler = end_element
    p.CharacterDataHandler = char_data

    p.ParseFile(fsock)
    #p.Parse("<p>cats &amp; dogs</p>")

def main():
    args = sys.argv[1:]
    if len(args) != 1:
        print 'usage: python expat.py infile.xml'
        sys.exit(-1)
    test(args[0])

if __name__ == '__main__':
    main()
