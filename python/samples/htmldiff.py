import sys, os, difflib
import webbrowser


fromlines = open('diff1.txt', 'U').readlines()
tolines = open('diff2.txt', 'U').readlines()

diff = difflib.HtmlDiff().make_file(fromlines, tolines, 'diff1.txt', 'diff2.txt')

# we're using writelines because diff is a generator
#sys.stdout.writelines(diff)

fsock = open('diffs.html', "wb")
fsock.writelines(diff)
fsock.close()

webbrowser.open('diffs.html', new=1)
