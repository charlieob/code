from __future__ import division
import math
# import pprint

class DN:
    def __init__(self, real=0.0, dual=0.0):
        self.real = real
        self.dual = dual
        
    def __repr__(self):
        return '(%f,%f)' % (self.real, self.dual)
        
    def __add__(self, dn):
        return DN(self.real+dn.real, self.dual+dn.dual)
        
    def __sub__(self, dn):
        return DN(self.real-dn.real, self.dual-dn.dual)
        
    def __mul__(self, dn):
        return DN(self.real*dn.real, self.real*dn.dual + self.dual*dn.real)
        
    def __truediv__(self, dn):
        return DN(self.real/dn.real, (self.dual*dn.real - self.real*dn.dual)/(dn.real*dn.real))
        
    def sqrt(self):
        return DN(math.sqrt(self.real), 0.5*self.dual/math.sqrt(self.real))
        
    def __pow__(self, n):
        """ implemented using the Taylor/Maclaurin expansion of (a+b)^r """
        return DN(math.pow(self.real, n), n*self.dual*math.pow(self.real,n-1))
        
    def sin(self):
        return DN(math.sin(self.real), self.dual * math.cos(self.real))
        
    def cos(self):
        return DN(math.cos(self.real), -self.dual * math.sin(self.real))
        
    def tan(self):
        return DN(math.tan(self.real), self.dual / math.cos(self.real)**2)
        
    def smoothstep(self):
        """ well-known sigmoidal step function used in computer graphics:
            f(x)  = 3x^2 - 2x^3
            f'(x) = 6x - 6x^2
        """
        return self * self * (DN(3) - DN(2) * self);

def TestSmoothStep(x):
    y = DN(x,1).smoothstep()
    print "smoothstep 3x^2-2x^3(%0.4f)  = %0.4f" % (x, y.real)
    print "smoothstep 3x^2-2x^3(%0.4f)' = %0.4f" % (x, y.dual)

def TestTrig(x):
    y = DN(x, 1).sin()
    print "sin(%0.4f)  = %0.4f" % (x, y.real)
    print "sin(%0.4f)' = %0.4f" % (x, y.dual)

    y = DN(x, 1).cos()
    print "cos(%0.4f)  = %0.4f" % (x, y.real)
    print "cos(%0.4f)' = %0.4f" % (x, y.dual)

    y = DN(x, 1).tan()
    print "tan(%0.4f)  = %0.4f" % (x, y.real)
    print "tan(%0.4f)' = %0.4f" % (x, y.dual)

def TestSimple(x):
    y = DN(3) / DN(x,1).sqrt()
    print "3/sqrt(%0.4f)  = %0.4f" % (x, y.real)
    print "3/sqrt(%0.4f)' = %0.4f" % (x, y.dual)

    y = (DN(x,1) + DN(1)) ** 1.337
    print "(%0.4f+1)^1.337  = %0.4f" % (x, y.real)
    print "(%0.4f+1)^1.337' = %0.4f" % (x, y.dual)


# print math.pi
# print DN()
# print DN(1)
# print DN(dual=1)
# print DN(1,2)
# print DN(1,2) + DN(1,1)
# print DN(1.0,1.0) / DN(2.0,4.0)
# print DN(1,1) / DN(2,4)
# s = (DN(5,6)).sqrt()
# print s
# print s*s
# print s**2
# print s**2.7

TestSmoothStep(0.5)
TestSmoothStep(0.75)
TestTrig(math.pi * 0.25)
TestSimple(3)
