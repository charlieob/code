'''
simple example of a generator
taken from "Dive into Python", p. 245
'''

def fibonacci(max):
    a, b = 0, 1
    while a < max:
        yield a
        a, b = b, a+b

for n in fibonacci(5000):
    print n,
