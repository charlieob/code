#
# See http://stackoverflow.com/questions/36636/what-is-a-closure
#
# Example showing three different variable scopes available in the closure:
# (1) parameter of function that creates the closure (a)
# (2) local variable of the creating function
# (3) parameter of the closure function when it is called
#

def outer(a):
    b = 'variable in outer()'
    def inner(c):
            print a,b,c
    return inner

f = outer('test')
f(1)
