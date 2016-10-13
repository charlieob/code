# http://richardwiseman.wordpress.com/2011/04/15/its-the-friday-puzzle-107/
#
# Can you cross out the unnecessary letters to leave a common phrase?
#
# ATCHEOUNMNE
# MCESOSNAPRH
# YRLEATTSEERS
#

s = "ATCHEOUNMNE" + "MCESOSNAPRH" + "YRLEATTSEERS"
print s
drop = "THEUNNECESSARYLETTERS"
ans = ""

for c in drop:
    (s1, s2) = s.split(c, 1)
    ans += s1
    s = s2

print ans