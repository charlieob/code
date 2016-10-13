import sys, re

#pat = re.compile('aa([0-9]*)bb')
pat = re.compile('aa(\d*)bb')

while 1:
    line = raw_input('Enter a line ("q" to quit):')
    if line == 'q':
        break
    mo = pat.search(line)
    if mo:
        value = mo.group(1)
        print 'value: %s' % value
    else:
        print 'no match'
