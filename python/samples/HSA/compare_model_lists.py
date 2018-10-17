import sys
import xml.etree.ElementTree as etree

def main(args):

    tree = etree.parse(args[0])
    root = tree.getroot()
    models1 = {}
    for child in root:
        models1[child.text] = child.attrib['style']
    
    tree = etree.parse(args[1])
    root = tree.getroot()
    models2 = {}
    for child in root:
        models2[child.text] = child.attrib['style']

    for m in sorted(models1):
        if m in models2:
            if models1[m] != models2[m]:
                print("{}: was {}, now {}". format(m, models1[m], models2[m]))
        else:
            print("{}: missing from new list". format(m))

if __name__=='__main__':

    if len(sys.argv) != 3:
        usage()
        sys.exit(2)

    main(sys.argv[1:])
