import glob
# glob supports Unix style pathname expansion
python_files = glob.glob('*.py')
print python_files
for fn in sorted(python_files):
    print '    ------', fn
#    print 'open(fn):', list(open(fn))
    for line in open(fn):
        print '    ' + line.rstrip()
    print
