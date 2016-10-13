"""Directory tree listing with indentation

This program was written by Charlie O'Brien
as part of learning how to use python
October 2008
"""

import os

def dirtree(path, indent=0):        
    """Print directory tree with indentation

    Ignores all directories whose name starts with '.'"""
    ls = [el for el in os.listdir(path) if el[0] <> '.']
    dirs = [d for d in ls if os.path.isdir(os.path.join(path, d))]
    for i in sorted(dirs):
        print i.rjust(len(i) + indent*4)
        dirtree(os.path.join(path, i), indent+1) #recursive call

if __name__ == "__main__":
    print "running as main"
    dirtree(os.curdir)

