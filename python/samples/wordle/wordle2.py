import string

BLACK = 0       # a miss
YELLOW = 1
GREEN = 2

def convert_score(c):
    if c == 'x':
        return GREEN
    elif c == 'o':
        return YELLOW
    else:
        return BLACK

def score_word(target, word):
    base = ord('a')

    # an array counting the occurrences of each letter in target word
    count = [0] * 26
    for c in target:
        count[ord(c)-base] += 1

    # each time we find a match (exact position or not) we decrement the count for that character
    score = []
    for i in range(5):
        c1, c2 = target[i], word[i]
        if c2 not in target:
            s = BLACK
        elif c2 == c1:
            s = GREEN
            count[ord(c2)-base] -= 1
        elif count[ord(c2)-base] > 0:
            s = YELLOW
            count[ord(c2)-base] -= 1
        else:
            s = BLACK
        score.append(s)
    return score

def main():
    words = []
    with open("words.txt") as f:
        for w in f:
            words.append(w.strip())
    # print("There are {} words in the list.".format(len(words)))

    data = []
    with open("guesses.txt") as f:
        for line in f:
            word, result = line.split()
            if not (len(word) == 5 and set(word) <= set(string.ascii_lowercase) and len(result) == 5 and set(result) <= set ("-ox")):
                print("badly formatted input here: " + line)
                exit(1)
            scores = [convert_score(c) for c in result]
            data.append((word, scores))

    for w in words:
        match = True
        for d in data:
            if score_word(w, d[0]) != d[1]:
                match = False
                break
        if match:
            print(w)

if __name__ == "__main__":
    main()
