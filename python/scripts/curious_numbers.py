# http://www.johndcook.com/blog/2016/02/15/curious-numbers/
for i in range(2, 20):
    a = 5**(2**i) % 10**i
    b = 10**i - a + 1
    print((a**2 - a)%10**i, (b**2 - b)%10**i, a, b)
