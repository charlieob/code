# recursive diff of all the files in two directories
# also reports any file present in one tree and not in the other

import sys
import os
import difflib
import pdb


def usage():
    print "usage: diff_dir.py dir1 dir2"


def main(args):

    # extract command line args
    # also add a path separator at the end, if not already there
    dir1 = os.path.join(os.path.normpath(args[0]), '')
    dir2 = os.path.join(os.path.normpath(args[1]), '')

    list1 = []
    for root, dirs, files in os.walk(dir1):
        for name in files:
            list1.append(os.path.join(root.replace(dir1,'', 1), name))

    list2 = []
    for root, dirs, files in os.walk(dir2):
        for name in files:
            list2.append(os.path.join(root.replace(dir2,'', 1), name))

    only1 = set(list1) - set(list2)
    only2 = set(list2) - set(list1)

    if len(only1) > 0:
        print '%d files only in %s:' % (len(only1), args[0])
        for name in only1:
            print name
            
            # and now remove it from the list since we don't want to diff it later
            list1.remove(name)

    if len(only2) > 0:
        print '%d files only in %s:' % (len(only2), args[1])
        for name in only2:
            print name

    for name in sorted(list1):
    
        #pdb.set_trace()
        file1 = os.path.join(dir1, name)
        file2 = os.path.join(dir2, name)

        text1 = open(file1, "r").readlines()
        text2 = open(file2, "r").readlines()

        diffs = difflib.unified_diff(text1, text2, file1, file2, n=0)
        #diffs = difflib.HtmlDiff().make_file(text1, text2, file1, file2)
        
        # some custom stuff follows, for handling a specific set of differences...
        #
        #for line in [d for d in diffs if not d.startswith(('---','+++','@@'))
        #                and not "TPT GENERATOR VERSION" in d
        #                and not "CODE GENERATED" in d
        #                and not d.startswith('+#')]:

        for line in diffs:
            sys.stdout.write(line)


if __name__=='__main__':

    if len(sys.argv) != 3:
        usage()
        sys.exit(2)

    main(sys.argv[1:])
