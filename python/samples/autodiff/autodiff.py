# simple, naive implementation
#  - evaluation
#  - forward mode automatic differentiation, diff()
#  - reverse mode, reverse_diff()
# 
# intention here is to show how the calculation works, and
# not to show how it could be efficiently implemented
# 

from __future__ import division
import math

class Const:
    def __init__(self, val):
        self.val = val
        self.adjoint = 0
        
    def eval(self):
        return self.val
        
    def diff(self, x):
        self.deriv = 0
        return 0
    
    def reverse_diff(self, a):
        self.adjoint += a
        
    def __repr__(self, indent=0):
        return '%s%f' % (' '*indent, self.val)
        
class Var:
    def __init__(self, name):
        self.name = name
        self.adjoint = 0
        
    def eval(self):
        self.val = vars[self.name]
        return self.val

    def diff(self, x):
        self.deriv = 1 if self.name == x else 0
        return self.deriv
        
    def reverse_diff(self, a):
        self.adjoint += a
        
    def __repr__(self, indent=0):
        return '%s%s' % (' '*indent, self.name)
        
class Add:
    def __init__(self, left, right):
        self.left = left
        self.right = right
        self.adjoint = 0
        
    def eval(self):
        self.val = self.left.eval() + self.right.eval()
        return self.val
        
    def diff(self, x):
        self.deriv = self.left.diff(x) + self.right.diff(x)
        return self.deriv
        
    def reverse_diff(self, a):
        self.adjoint += a
        self.left.reverse_diff(a)
        self.right.reverse_diff(a)
        
    def __repr__(self, indent=0):
        return '%s+\n%s\n%s' % (' '*indent, self.left.__repr__(indent+1), self.right.__repr__(indent+1))
        
class Sub:
    def __init__(self, left, right):
        self.left = left
        self.right = right
        self.adjoint = 0
        
    def eval(self):
        self.val = self.left.eval() - self.right.eval()
        return self.val

    def diff(self, x):
        self.deriv = self.left.diff(x) - self.right.diff(x)
        return self.deriv
        
    def reverse_diff(self, a):
        self.adjoint += a
        self.left.reverse_diff(a)
        self.right.reverse_diff(-a)
        
    def __repr__(self, indent=0):
        return '%s-\n%s\n%s' % (' '*indent, self.left.__repr__(indent+1), self.right.__repr__(indent+1))
        
class Mul:
    def __init__(self, left, right):
        self.left = left
        self.right = right
        self.adjoint = 0
        
    def eval(self):
        self.val = self.left.eval() * self.right.eval()
        return self.val
        
    def diff(self, x):
        self.deriv = self.left.val * self.right.diff(x) + self.right.val * self.left.diff(x)
        return self.deriv
        
    def reverse_diff(self, a):
        self.adjoint += a
        self.left.reverse_diff(a * self.right.val)
        self.right.reverse_diff(a * self.left.val)
        
    def __repr__(self, indent=0):
        return '%s*\n%s\n%s' % (' '*indent, self.left.__repr__(indent+1), self.right.__repr__(indent+1))
        
class Div:
    def __init__(self, left, right):
        self.left = left
        self.right = right
        self.adjoint = 0
        
    def eval(self):
        self.val = self.left.eval() / self.right.eval()
        return self.val

    def diff(self, x):
        self.deriv = (self.right.val * self.left.diff(x) - self.left.val * self.right.diff(x)) / (self.right.val ** 2)
        return self.deriv
        
    def reverse_diff(self, a):
        self.adjoint += a
        self.left.reverse_diff(a / self.right.val)
        self.right.reverse_diff(-a / (self.right.val ** 2))
        
    def __repr__(self, indent=0):
        return '%s/\n%s\n%s' % (' '*indent, self.left.__repr__(indent+1), self.right.__repr__(indent+1))
        
class Pow:
    def __init__(self, base, exp):
        self.base = base
        self.exp = exp
        self.adjoint = 0
        
    def eval(self):
        self.val = self.base.eval() ** self.exp.eval()
        return self.val
        
    def diff(self, x):
        if self.base.val > 0:
        # f(x) = u(x)^v(x), u(x) and v(x) both functions of x
        # what is df/dx ?
        # D_x[u ^ v] = D_x[exp(v ln u)]
        #            = exp(v ln u) D_x[v ln u]           (chain rule)
        #            = exp(v ln u) (v'ln u + vu' / u)
        #            = u ^ v (uv'ln u + vu') / u
        #            = u ^ (v-1) (uv' ln u + vu')
            self.deriv = self.base.val ** (self.exp.val-1) *\
                    (self.base.val*self.exp.diff(x)*math.log(self.base.val) +\
                     self.exp.val*self.base.diff(x))
        else:
        # drop the ln() component to avoid computational error
        # since the log of a negative number is complex
            self.deriv = self.base.val ** (self.exp.val-1) *\
                     self.exp.val*self.base.diff(x)
        return self.deriv
        
    def reverse_diff(self, a):
        self.adjoint += a
        self.base.reverse_diff(a * self.exp.val * self.base.val ** (self.exp.val-1))
        if self.base.val > 0:
            self.exp.reverse_diff(a * math.log(self.base.val) * self.base.val ** self.exp.val)
        else:
            self.exp.reverse_diff(a)
        
    def __repr__(self, indent=0):
        return '%s^\n%s\n%s' % (' '*indent, self.base.__repr__(indent+1), self.exp.__repr__(indent+1))
        
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
# print w7

vars = {'x1': 1.0, 'x2': 2.0}

assert w7.eval() == f(vars['x1'], vars['x2'])                         # 21.5
w7.reverse_diff(1.0)

assert w7.diff('x1') == w1.adjoint == f_x(vars['x1'], vars['x2'])     # 22.5
assert w7.diff('x2') == w2.adjoint == f_y(vars['x1'], vars['x2'])     # 9.75

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

vars = {'p': -1.0, 'q': 2.5, 'r': 1.5}

assert w7.eval() == f(vars['p'], vars['q'], vars['r'])      # -72.625
w7.reverse_diff(1.0)

assert w7.diff('p') == p.adjoint == f_x(vars['p'], vars['q'], vars['r']) # 49.875
assert w7.diff('q') == q.adjoint == f_y(vars['p'], vars['q'], vars['r']) # -46.5
assert w7.diff('r') == r.adjoint == f_z(vars['p'], vars['q'], vars['r']) # -49.25

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

vars = {'p': -1.0, 'q': 2.5, 'r': 1.5}

assert w7.eval() == f(vars['p'], vars['q'], vars['r'])      # -72.625
w7.reverse_diff(1.0)

assert w7.diff('p') == p.adjoint == f_x(vars['p'], vars['q'], vars['r']) # 49.875
assert w7.diff('q') == q.adjoint == f_y(vars['p'], vars['q'], vars['r']) # -46.5
assert w7.diff('r') == r.adjoint == f_z(vars['p'], vars['q'], vars['r']) # -49.25

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

vars = {'p': -1.0, 'q': 2.5, 'r': 1.5}

assert w7.eval() == f(vars['p'], vars['q'], vars['r'])      # -72.625
w7.reverse_diff(1.0)

assert w7.diff('p') == p.adjoint == f_x(vars['p'], vars['q'], vars['r']) # 49.875
assert w7.diff('q') == q.adjoint == f_y(vars['p'], vars['q'], vars['r']) # -46.5
assert w7.diff('r') == r.adjoint == f_z(vars['p'], vars['q'], vars['r']) # -49.25

#
# we're done now, no exceptions raised by the assert statements
#
print "all checks passed"