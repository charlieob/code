# simple, naive attempt to perform forward mode automatic differentiation using dual numbers
# 

from __future__ import division
import math

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
        
class Const:
    def __init__(self, val):
        self.val = val
        
    def eval(self):
        return DN(self.val)
        
    def __repr__(self, indent=0):
        return '%s%f' % (' '*indent, self.val)
        
class Var:
    def __init__(self, name):
        self.name = name
        
    def eval(self):
        self.val = vars[self.name]
        return self.val

    def __repr__(self, indent=0):
        return '%s%s' % (' '*indent, self.name)
        
class Add:
    def __init__(self, left, right):
        self.left = left
        self.right = right
        
    def eval(self):
        self.val = self.left.eval() + self.right.eval()
        return self.val
        
    def __repr__(self, indent=0):
        return '%s+\n%s\n%s' % (' '*indent, self.left.__repr__(indent+1), self.right.__repr__(indent+1))
        
class Sub:
    def __init__(self, left, right):
        self.left = left
        self.right = right
        
    def eval(self):
        self.val = self.left.eval() - self.right.eval()
        return self.val

    def __repr__(self, indent=0):
        return '%s-\n%s\n%s' % (' '*indent, self.left.__repr__(indent+1), self.right.__repr__(indent+1))
        
class Mul:
    def __init__(self, left, right):
        self.left = left
        self.right = right
        
    def eval(self):
        self.val = self.left.eval() * self.right.eval()
        return self.val
        
    def __repr__(self, indent=0):
        return '%s*\n%s\n%s' % (' '*indent, self.left.__repr__(indent+1), self.right.__repr__(indent+1))
        
class Div:
    def __init__(self, left, right):
        self.left = left
        self.right = right
        
    def eval(self):
        self.val = self.left.eval() / self.right.eval()
        return self.val

    def __repr__(self, indent=0):
        return '%s/\n%s\n%s' % (' '*indent, self.left.__repr__(indent+1), self.right.__repr__(indent+1))
        
class Pow:
    def __init__(self, base, exp):
        self.base = base
        self.exp = exp
        
    def eval(self):
        self.val = self.base.eval() ** self.exp.eval().real
        return self.val
        
    def __repr__(self, indent=0):
        return '%s^\n%s\n%s' % (' '*indent, self.base.__repr__(indent+1), self.exp.__repr__(indent+1))
        
        
# w1 = Var('x')
# w2 = Add(w1, Const(5))

# vars = {'x': DN(10,1)}

# print w2.eval()

# EXAMPLE 1
#
# f(x,y) = x^2 +10xy + x/y
f = lambda x,y: x**2 + 10*x*y + x/y

# partial derivatives of f(x,y)
f_x = lambda x,y: 2*x + 10*y +1/y
f_y = lambda x,y: 10*x - x/y**2

w1 = Var('x1')
w2 = Var('x2')
w3 = Mul(w1, w1)
w4 = Mul(w1, w2)
w5 = Div(w1, w2)
w6 = Add(Mul(w4, Const(10)), w5)
w7 = Add(w3, w6)

vars = {'x1': DN(1.0, 1), 'x2': DN(2.0)}
d1 = w7.eval()
vars = {'x1': DN(1.0), 'x2': DN(2.0, 1)}
d2 = w7.eval()

assert d1.real == f(vars['x1'].real, vars['x2'].real)         # 21.5
assert d1.dual == f_x(vars['x1'].real, vars['x2'].real)       # 22.5
assert d2.dual == f_y(vars['x1'].real, vars['x2'].real)       # 9.75

# EXAMPLE 2
#
# f(x,y,z) = x^3 + 5xy^2z - 6yz - z^2
f = lambda x,y,z: x**3 + 5*x*y**2*z - 6*y*z - z**2

# partial derivatives of f(x,y,z)
f_x = lambda x,y,z: 3*x**2 + 5*y**2*z
f_y = lambda x,y,z: 10*x*y*z - 6*z
f_z = lambda x,y,z: 5*x*y**2 - 6*y - 2*z

p = Var('p')
q = Var('q')
r = Var('r')
w1 = Pow(p, Const(3))
w2 = Mul(p, Mul(Pow(q, Const(2)), r))
w3 = Mul(q, r)
w4 = Pow(r, Const(2))
w5 = Add(w1, Mul(Const(5), w2))
w6 = Add(Mul(Const(6), w3), w4)
w7 = Sub(w5, w6)
# print w7

vars = {'p': DN(-1.0, 1), 'q': DN(2.5), 'r': DN(1.5)}
d1 = w7.eval()
vars = {'p': DN(-1.0), 'q': DN(2.5, 1), 'r': DN(1.5)}
d2 = w7.eval()
vars = {'p': DN(-1.0), 'q': DN(2.5), 'r': DN(1.5, 1)}
d3 = w7.eval()

assert d1.real == f(vars['p'].real, vars['q'].real, vars['r'].real)           #  -72.625
assert d1.dual == f_x(vars['p'].real, vars['q'].real, vars['r'].real)         # 49.875
assert d2.dual == f_y(vars['p'].real, vars['q'].real, vars['r'].real)         # -46.5
assert d3.dual == f_z(vars['p'].real, vars['q'].real, vars['r'].real)         # -49.25

# EXAMPLE 2a
# same as above but without using the power operator in the graph
# f(x,y,z) = x^3 + 5xy^2z - 6yz - z^2

p = Var('p')
q = Var('q')
r = Var('r')
w1a = Mul(p, Mul(p, p))
w2 = Mul(p, Mul(Mul(q, q), r))
w3 = Mul(q, r)
w4 = Mul(r, r)
w5 = Add(w1a, Mul(Const(5), w2))
w6 = Add(Mul(Const(6), w3), w4)
w7 = Sub(w5, w6)

vars = {'p': DN(-1.0, 1), 'q': DN(2.5), 'r': DN(1.5)}
d1 = w7.eval()
vars = {'p': DN(-1.0), 'q': DN(2.5, 1), 'r': DN(1.5)}
d2 = w7.eval()
vars = {'p': DN(-1.0), 'q': DN(2.5), 'r': DN(1.5, 1)}
d3 = w7.eval()

assert d1.real == f(vars['p'].real, vars['q'].real, vars['r'].real)           #  -72.625
assert d1.dual == f_x(vars['p'].real, vars['q'].real, vars['r'].real)         # 49.875
assert d2.dual == f_y(vars['p'].real, vars['q'].real, vars['r'].real)         # -46.5
assert d3.dual == f_z(vars['p'].real, vars['q'].real, vars['r'].real)         # -49.25

# EXAMPLE 2b
# same as 2a but with some refactoring of the expression to introduce fanout in
# the middle of the graph, and not just at the input nodes.
# f(x,y,z) = x(x^2 + 5y^2z) - z(6y + z)

p = Var('p')
q = Var('q')
r = Var('r')
w1a = Mul(p, Mul(p, p))
w2 = Mul(p, Mul(Mul(q, q), r))
w3 = Mul(q, r)
w4 = Mul(r, r)
w5 = Add(w1a, Mul(Const(5), w2))
w6 = Add(Mul(Const(6), w3), w4)

w1 = Mul(p, p)
w2 = Mul(Mul(q, q), r)
w3 = Add(w1, Mul(Const(5), w2))
w4 = Mul(p, w3)
w5 = Add(Mul(Const(6), q), r)
w6 = Mul(r, w5)
w7 = Sub(w4, w6)

vars = {'p': DN(-1.0, 1), 'q': DN(2.5), 'r': DN(1.5)}
d1 = w7.eval()
vars = {'p': DN(-1.0), 'q': DN(2.5, 1), 'r': DN(1.5)}
d2 = w7.eval()
vars = {'p': DN(-1.0), 'q': DN(2.5), 'r': DN(1.5, 1)}
d3 = w7.eval()

assert d1.real == f(vars['p'].real, vars['q'].real, vars['r'].real)           #  -72.625
assert d1.dual == f_x(vars['p'].real, vars['q'].real, vars['r'].real)         # 49.875
assert d2.dual == f_y(vars['p'].real, vars['q'].real, vars['r'].real)         # -46.5
assert d3.dual == f_z(vars['p'].real, vars['q'].real, vars['r'].real)         # -49.25

#
# we're done now, no exceptions raised by the assert statements
#
print "all checks passed"