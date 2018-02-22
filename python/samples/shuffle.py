# https://www.johndcook.com/blog/2018/01/23/distribution-of-matches-between-two-shuffled-decks/

import numpy as np
from scipy.stats import poisson
import matplotlib.pyplot as plt

def count_zeros(x):
    return len(x[x==0])

num_reps = 10000
deck_size = 52

matches = np.zeros(deck_size+1, dtype=int)

# Simulation
for _ in range(num_reps):

    # Shuffle two decks
    a = np.random.permutation(deck_size)
    b = np.random.permutation(deck_size)

    # Count how often they match
    num_matches = count_zeros(a-b)
    matches[ num_matches ] += 1

# Cut off outputs too small to see
cutoff = 8

# Matches predicted by a Poisson distribution with mean 1.
predicted = [num_reps*poisson(1).pmf(i) for i in range(cutoff)]

# Plot results
x = np.arange(cutoff)
w = 0.3 # bar width
plt.bar(x, matches[0:cutoff], w)
plt.bar(x+w, predicted, w)
plt.legend(["actual", "predicted"])
plt.xlabel("matches")
plt.ylabel("frequency")
plt.savefig("shuffled_desk_matches.svg")
plt.show()