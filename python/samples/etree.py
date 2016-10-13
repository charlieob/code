#!/usr/bin/python
#############################################################################
#  Copyright (c) 2009 Silicon & Software Systems Limited
#  Whelan House, South County Business Park,
#  Leopardstown, Dublin 18, Ireland
#
#  All Rights Reserved
#
#  P R O P R I E T A R Y    &    C O N F I D E N T I A L
#
#  The copyright of this document is exclusively vested in Silicon & Software Systems Limited, without whose prior
#  written permission its contents must not be published,  adapted or reproduced in any form or by any means or
#  disclosed or issued to any third party.
#  -----------------------------------------------------
#  http://www.s3group.com
#  -----------------------------------------------------
#
#  MODULE: qc_xml.py
#
#  DESCRIPTION: Starting point for a Python program which can parse Quality Center XML export
#               and "understand" the test steps
#
#               It should be obvious that this could be expanded to translate "Press the SELECT key" into
#               the appropriate StormTest script command to send the appropriate IR key, for instance.
#
###############################################################################


##############################################################################
# Import modules
#################################################################################

import sys
#import xml.etree.ElementTree as etree    # cElementTree has the same API but better performance
import xml.etree.cElementTree as etree
from htmlstripper import strip_tags

def walk_tree(node, level):
    fill = show_level(level)

    if node.keys():
        attrs = ", ".join(["%s:%s" % (name,repr(value)) for name, value in node.items()])
    else:
        attrs = ""

    text = repr(node.text) + ', ' + repr(node.tail)
    #if node.tail:
    #    text = text + ', ' + repr(node.tail)
        
    print '%s%s [%s] %s' % (fill, node.tag, attrs, text)
    
    #print '%sElement: %s' % (fill, node.tag, )
    #print '%s  text: %s' % (fill, node.text)
    #for (name, value) in node.items():
    #    print '%s    Attr -- %s : %s' % (fill, name, value,)
    if node.attrib.get('ID') is not None:
        print '%s    ID: %s' % (fill, node.attrib.get('ID').value, )
    children = node.getchildren()
    for child in children:
        walk_tree(child, level + 1)

def show_level(level):
    s1 = '    ' * level
    return s1

def parseXML(inFileName, cases):
    ''' a simple parser... '''
    
    elemTree = etree.parse(inFileName)
    #print 'parse() completed'
    rootElem = elemTree.getroot()
    
    elemIter = rootElem.getiterator('Record')  # elements in order they appeared in XML section
    
    allCases = {}

    # each item of the iterator will be a 'Record' element from the XML source
    for rec in elemIter:
        testID = int(rec.find("DS_TEST_ID").text)

        if allCases.has_key(testID):
            testSteps = allCases[testID]
            stepNameElem = rec.find("DS_STEP_NAME")
            if stepNameElem.text == "Environment":
                testSteps.insert(0, rec)
            else:
                testSteps.append(rec)
        else:
            testSteps = [rec]
            allCases[testID] = testSteps
    
    if cases == []:
        testCases = allCases
    else:
        testCases = {}
        for c in cases:
            ic = int(c)
            if allCases.has_key(ic):
                testCases[ic] = allCases[ic]

    print 'total number of test cases found: ', len(allCases)
    print 'processing %d of those ... ' % len(testCases)
    #print sys.getdefaultencoding()
    
    for testID in testCases.iterkeys():

        testCaseRecs = testCases[testID]
        testName = testCaseRecs[0].find("TS_NAME").text
        testDescr = testCaseRecs[0].find("TS_DESCRIPTION").text
        print '---------------------'
        print '<Test ID> ', testID
        print '<Test Name> ', testName
        print '<Test Description>\n', strip_tags(testDescr)

        for testStep in testCaseRecs:
            print '-----'
            stepName = testStep.find("DS_STEP_NAME").text
            stepDescr = testStep.find("DS_DESCRIPTION").text
            stepExpected = testStep.find("DS_EXPECTED").text
            print '<Step Name> ', stepName
            print '<Action>\n', strip_tags(stepDescr)
            print '<Expected>\n', strip_tags(stepExpected)

def main():
    args = sys.argv[1:]
    if len(args) < 1:
        print 'usage: python etree.py infile.xml [test case ID(s)]'
        sys.exit(-1)
    elif len(args) == 1:
        parseXML(args[0], [])
    else:
        parseXML(args[0], args[1:])

if __name__ == '__main__':
    main()
