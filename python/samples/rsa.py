# investigation of the RSA encyption scheme
# essence of the scheme is modular exponentiation for which Python provides a built-in function
#       pow(x,y,z)
# which performs it efficiently
# 

def primes(n):
    """ Returns  a list of primes < n
        naive implementation as in https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes """
    sieve = [True] * n
    for i in xrange(2, int(n**0.5)+1):
        if sieve[i]:
            sieve[i*i::i] = [False]*((n-i*i+i-1)/(i))
    return [i for i in xrange(2,n) if sieve[i]]

def primes2(n):
    """ Returns  a list of primes < n
        more efficient """
    sieve = [True] * n
    for i in xrange(3, int(n**0.5)+1, 2):
        if sieve[i]:
            sieve[i*i::2*i] = [False]*((n-i*i-1)/(2*i)+1)
    return [2] + [i for i in xrange(3,n,2) if sieve[i]]

def gcd(x, y):
    while y != 0:
        (x, y) = (y, x % y)
    return x

l = primes(1000000)

# select 2 of these prime numbers, of similar size
p,q = l[-1], l[-5000]
n = p*q
print "n = %d = %d * %d" % (n,p,q)
phi_n = (p-1) * (q-1)
print "phi(n) =", phi_n

# common choices for e are 3, 17 and 65537
# requirement on e is that it must be coprime with phi(n) and less than phi(n)
e = 3
if gcd(e,phi_n) != 1:
    e = 17
if gcd(e,phi_n) != 1:
    print "need help here with choosing e !?"
    exit()

print "e =", e

k = 2
d = (k*phi_n + 1) / e
# must choose k so that d is an integer
while d*e != (k*phi_n + 1):
    k += 1
    d = (k*phi_n + 1) / e

print "k =", k
print "d =", d

# ENCRYPTION
# public key is (e, n)

m = 1234567890
print "\nm =", m

print "encrypting message text m with the public key (e,n) ..."

c = pow(m,e,n)
print "c =", c

# DECRYPTION
# private key is d

print "decrypting cipher text c with the private key (d,n)..."

cc = pow(c,d,n)
print "m =", cc

print "encryption/decryption was %s" % ("SUCCESSFUL" if cc == m else "UNSUCCESSFUL")

# DIGITAL SIGNATURE
# sign the message with private key

m = 9876543210
print "\nm =", m

print "signing message text m with the private key (d,n) ..."

s = pow(m,d,n)
print "s =", s

# SIGNATURE VERIFICATION
# verify the signature using the public key

print "verifying signature s of m with the public key (e,n) ..."

ss = pow(s,e,n)
print "m =", ss

print "signature verification was %s" % ("SUCCESSFUL" if ss == m else "UNSUCCESSFUL")
