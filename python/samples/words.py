fp = open('wordsEn.txt')
words = [l.strip() for l in fp.readlines()]
fp.close()

letters = set('asdfghjkl')
w2 = [w for w in words if set(w).issubset(letters)]
print sorted(w2, key=len, reverse=True)
