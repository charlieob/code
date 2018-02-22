import sys

print sys.maxint
n = sys.maxint
print format(n, '#x')
print format(n, 'x')
print format(n, '#b')
print format(n, 'b')

print n*n*n*n*n*n*n*n*n*n
print n+1
print n-1

print

print("n = 0x%x, %s" % (n, str(type(n))))
n += 1
print("n = 0x%x, %s" % (n, str(type(n))))
n -= 1
print("n = 0x%x, %s" % (n, str(type(n))))

print

n -= sys.maxint
# n=0 now but what type is it?
print ("n = %d, %s" % (n, str(type(n))))
# still a long
print type(int(n))

print

# python 'int' type is an abstraction of a signed integer
# python automatically promotes from int to long (which has unlimited precision) as required
#
# note that in python 3 the two types are unified and there is no such thing as sys.maxint

# find max int without using sys.maxint (python 2)
n = 1
while 'long' not in str(type(n+1)):
    n = (n<<1)+1
print ("maxint = %d (0x%x)" % (n, n))

print
# construct the number maxint+1
m = int('1' + '0'*len(format(n, 'b')), 2)
print format(m, '#b')
print format(m, '#x')

