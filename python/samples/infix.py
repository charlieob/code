import re

pattern = re.compile(r"""
    \s*
    (?:
        (\d+(?:.\d+)?)
        |([+\-*/()])
    )
    """, re.VERBOSE)

expr = " 2 + 3*(10 - 1.0)"

tokenlist = []
pos = 0
while 1:
    m = pattern.match(expr, pos)
    if not m:
        break
    print m.lastindex, repr(m.group(m.lastindex)), m.start(m.lastindex)
    pos = m.end()
