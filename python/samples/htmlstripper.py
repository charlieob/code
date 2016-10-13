from HTMLParser import HTMLParser
from xml.dom import minidom
from BeautifulSoup import BeautifulSoup 

class HTMLStripper(HTMLParser):
    def __init__(self):
        HTMLParser.__init__(self)
        self.pieces = []

    def handle_data(self, text):
    # called for each block of plain text
        self.pieces.append(text)

    def output(self):
        """Return processed HTML as a single string"""
        #pdb.set_trace()
        return "".join(self.pieces)

def extractPlainText(elem):
    pieces = []
    if elem.nodeType == elem.TEXT_NODE:
        pieces.append(elem.data)
    elif elem.nodeType == elem.ELEMENT_NODE:
        if elem.tagName in ['br', 'p']:
            pieces.append('\n')
        for child in elem.childNodes:
            pieces.append(extractPlainText(child))
    return "".join(pieces)


def htmlparser_strip_tags(html):
    # if the HTML parser doesn't recognise it as HTML, then it probably
    # isn't HTML, so we'll just assume it was plain text
    try:
        soup = BeautifulSoup(html)
        
        s = HTMLStripper()
        s.feed(str(soup))
        
        retval = s.output()
        
    except:
        #raise
        #print 'exception...'
        retval = html
    return retval

def strip_tags(html):
    # if the HTML parser doesn't recognise it as HTML, then it probably
    # isn't HTML, so we'll just assume it was plain text

    try:
        soup = BeautifulSoup(html)
        htmldoc = minidom.parseString(str(soup))
        bodyElems = htmldoc.getElementsByTagName("body")
        
        retval = extractPlainText(bodyElems[0])
        
    except:
        #raise
        #print 'exception...'
        retval = html
    return retval

if __name__ == "__main__":
    print 'unit tests...'
    htmltext = "<html><body>some text<br/><p>and some <b>more</b> text</p></body></html>"
    print strip_tags(htmltext)
    htmltext = "<html><body>this one is on\ntwo lines</body></html>"
    print strip_tags(htmltext)
    htmltext = '<html><body>Navigate to and highlight <font color="#0000FF">Event 1</font>.</body></html>'
    print strip_tags(htmltext)
    htmltext = '<html><body>Wait for 5 mins then<br>Replace <font color="#0000FF">Event 1 </font>with <font color="#0000FF">Event 2 </font>on headend</body></html>'
    #htmltext = '<html><body>Wait for 5 mins then BR Replace <font color="#0000FF">Event 1 </font>with <font color="#0000FF">Event 2 </font>on headend</body></html>'
    print strip_tags(htmltext)
    htmltext = "Call <Display Grid - ENTERTAINMENT-ALL>"
    print strip_tags(htmltext)
    htmltext = "Call <Grid- ENT>"
    print strip_tags(htmltext)
    htmltext = "Call <xyz - abc>"
    print strip_tags(htmltext)
    htmltext = "Call <xyz- abc>"
    print strip_tags(htmltext)

