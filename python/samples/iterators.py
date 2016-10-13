'''
Some sample code to understand how iterators work.
Invoking an iterator only works once - when it gets
to the end it raises the StopIteration exception and
just keeps doing that.
'''

it = iter([1,2,3,4])
print list(it)
print list(it)   # empty because the iterator has already been used/exhausted

it = iter(range(10,14))
print it
li = list(it)
print li
print li     # not exhausted because li is not an iterator
for i in li: print i,
print
print str(li)   # that is really just a string it's printed

it = iter(range(20,24))
print it.next()
print it.next()
print it.next()
print it.next()
#print it.next()    # that would cause an exception and script exit

it = iter(range(30,34))
while 1:
    try:
        print it.next()
    except:
        print 'oops, one too many'
        break
print list(it)
print it.next()    # this will fail and exit


