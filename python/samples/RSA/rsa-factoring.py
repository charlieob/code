# https://www.johndcook.com/blog/2018/08/23/rsa-numbers-and-factoring/
# need Python 3 for secrets module
# py -3 rsa-factoring.py

from secrets import randbits
from sympy import nextprime, gcd
from timeit import default_timer as timer

numbits = 2048

p = nextprime(randbits(numbits))
q = nextprime(randbits(numbits))
r = nextprime(randbits(numbits))

m = p*q
n = p*r

t0 = timer()
g = gcd(m, n)
assert(p == g)
assert(q == m//p)
assert(r == n//p)
t1 = timer()

# Print time in milliseconds
print(1000*(t1 - t0))
print(p)
print(q)
print(r)


# https://www.johndcook.com/blog/2018/08/23/rsa-numbers-and-factoring/

RSA232 = 17969491597941066732916128449573246156367561808012600070888918835531726460341490933493372247868650755230855864199929221814436684722874052065257937495694348389263171152522525654410980819170611742509702440718010364831638288518852689
p = 4528450358010492026612439739120166758911246047493700040073956759261590397250033699357694507193523000343088601688589
q = 3968132623150957588532394439049887341769533966621957829426966084093049516953598120833228447171744337427374763106901

if RSA232 == p*q:
    print("Check!")

