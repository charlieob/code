# http://www.johndcook.com/blog/2012/05/10/solutions-to-knights-random-walk/
from random import randint
 
# Move a knight from (x, y) to a random new position
def new_position(x, y):
 
    while True:
        dx, dy = 1, 2
 
        # it takes three bits to determine a random knight move:
        # (1, 2) vs (2, 1), and the sign of each
        r = randint(0, 7)
        if r % 2:
            dx, dy = dy, dx
        if (r >> 1) % 2:
            dx = -dx
        if (r >> 2) % 2:
            dy = -dy
 
        newx, newy = x + dx, y + dy
        # If the new position is on the board, take it.
        # Otherwise try again.
        if (newx >= 0 and newx < 8 and newy >= 0 and newy < 8):
            return (newx, newy)
 
# Count the number of steps in one random tour
def random_tour():
    x, y = x0, y0 = 0, 0
    count = 0
    while True:
        x, y = new_position(x, y)
        count += 1
        if x == x0 and y == y0:
            return count
 
# Average the length of many random tours
sum = 0
num_reps = 100000
for i in xrange(num_reps):
    sum += random_tour()
print sum / float(num_reps)
