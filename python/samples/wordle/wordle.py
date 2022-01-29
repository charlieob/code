import string

words = []
with open("words.txt") as f:
    for w in f:
        words.append(w.strip())
print("There are {} words in the list.".format(len(words)))

excluded = set("oxyz")
patterns = ["p-r-y", "p--ry"]

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
