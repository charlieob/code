#So, here is the puzzle.can you make the number 24 with the number 5, 5, 5, and 1 (again, you cannot join the numbers together, have to use each number once and only once, and are only allowed to add, subtract, multiply or divide them)?

s = "1%c5%c5%c5"
for op1 in ('+','-','*','/'):
    for op2 in ('+','-','*','/'):
        for op3 in ('+','-','*','/'):
            if eval(s % (op1,op2,op3)) == 24:
                print s

s = "5%c1%c5%c5"
for op1 in ('+','-','*','/'):
    for op2 in ('+','-','*','/'):
        for op3 in ('+','-','*','/'):
            if eval(s % (op1,op2,op3)) == 24:
                print s

s = "5%c5%c1%c5"
for op1 in ('+','-','*','/'):
    for op2 in ('+','-','*','/'):
        for op3 in ('+','-','*','/'):
            if eval(s % (op1,op2,op3)) == 24:
                print s

s = "5%c5%c5%c1"
for op1 in ('+','-','*','/'):
    for op2 in ('+','-','*','/'):
        for op3 in ('+','-','*','/'):
            if eval(s % (op1,op2,op3)) == 24:
                print s

