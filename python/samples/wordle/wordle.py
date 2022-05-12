import string

words = []
with open("words.txt") as f:
    for w in f:
        words.append(w.strip())
# print("There are {} words in the list.".format(len(words)))

excluded = set("insato")
patterns = ["-lder"]

charset = set(string.ascii_lowercase).difference(excluded)

for w in words:
    for p in patterns:
        match = True
        for i in range(5):
            if p[i] == '-':
                if w[i] not in charset:
                    match = False
            elif w[i] != p[i]:
                match = False
        if match:
            print(w)
            break

MISS = 0
MISPLACED = 1
EXACT = 2

def pattern_trit_generator(guess, true_word):
    for c1, c2 in zip(guess, true_word):
        if c1 == c2:
            yield EXACT
        elif c1 in true_word:
            yield MISPLACED
        else:
            yield MISS

def get_pattern(guess, true_word):
    """
    A unique integer id associated with the grey/yellow/green wordle
    pattern relatign a guess to the tue answer. In the ternary representation
    of this number, 0 -> grey, 1 -> yellow, 2 -> green.
    """
    return sum(
        value * (3**i)
        for i, value in enumerate(pattern_trit_generator(guess, true_word))
    )

print(list(enumerate(pattern_trit_generator("trace", "abcde"))))
print(get_pattern("trace", "abcde"))

