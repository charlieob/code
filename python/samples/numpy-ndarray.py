import numpy as np

a = np.array([1,2,3,4,5,6])
print("Type of a is %s" % type(a))
print a
print a.shape
a = np.array([[1,2],3,4,5,6])
print a
print a.shape
a = np.array([[1,2],[3,4],5,6])
print a
print a.shape
a = np.array([[1,2],[3,4],[5,6]])
print a
print a.shape

a = np.array([1,2,3,4])
b = a.reshape((4,1))
print "a =", a
print a.shape
print "b =", b
print b.shape
c = a[:, np.newaxis]
print "c =", c
print c.shape

# ndarray multiplication is element-wise with broadcasting
# https://docs.scipy.org/doc/numpy-1.13.0/user/basics.broadcasting.html
# http://scipy.github.io/old-wiki/pages/EricsBroadcastingDoc
#
print a*a   # matching sizes, element-wise multiplication, result same shape
print b*b
print a*b   # (1x4) x (4x1) yields (4x4) with broadcasting
print b*a   # identical
print np.dot(a,a)   # dot product of two 1D vectors, returns a scalar
# print np.dot(b,b) # fails
print np.dot(a,b)   # (1x4) x (4x1) matrix mult, returns a 1x1 matrix
# print np.dot(b,a) # fails

M = np.array([[1,2,3],[4,5,6]])
x = np.array([1,0,1])
print M             # (2x3) or M.shape=(2,3)
print x             # (3x1) or x.shape=(3,)
print M*x           # element-wise multiplication with broadcasting to x.shape=(2,3)
print (M*x).shape   # (2x3)
print np.dot(M,x)   # matrix multiplication
print np.dot(M,x).shape     # (2x1)
