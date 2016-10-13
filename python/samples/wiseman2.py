# And it's a tricky one this week. Yesterday I went shopping and picked up four items. When I got to the till, the cashier added the price of the four items together and the bill was pounds 7.11. I then noticed that I would get exactly the same total if I were to multiply the four prices. How much did each of the items cost?

N = 711

for i in range(1, N-2):
    for j in range(i, N-1-i):
        for k in range(j, N-i-j):
            if (i*j*k*(N-i-j-k) == N * 1000000):
                print i,j,k,N-i-j-k
