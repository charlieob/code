#
#
"""
http://en.wikipedia.org/wiki/Closure_(computer_science)

A closure (also lexical closure, function closure, function value or functional
value) is a function together with a referencing environment for the non-local
variables of that function. A closure allows a function to access variables
outside its typical scope. Such a function is said to be "closed over" its free
variables. The referencing environment binds the nonlocal names to the
corresponding variables in scope at the time the closure is created,
additionally extending their lifetime to at least as long as the lifetime of
the closure itself. When the closure is entered at a later time, possibly from
a different scope, the function is executed with its non-local variables
referring to the ones captured by the closure

Here is a simple closure, which requires Python 3 for the 'nonlocal'
declaration.

def accumulator(n):
    def inc(x):
        nonlocal n
        n += x
        return n
    return inc

The closure is the function inc() which is returned by a call to accumulator().

"""
#
# Here's an example that does something very similar by defining an object and
# making it callable using the __call__() method.
# See http://en.wikipedia.org/wiki/Function_object
#

class Accumulator(object):
    def __init__(self, n):
        self.n = n
    def __call__(self, x):
        self.n += x
        return self.n
        
a = Accumulator(4)
b = Accumulator(42)

print a(5)      # 9
print a(2)      # 11
print b(7)      # 49
