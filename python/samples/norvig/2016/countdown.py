from collections import defaultdict, Counter
import itertools

def evaluate(exp):
    "eval exp, or return None if there is an arithmetic error."
    try:
        return eval(exp)
    except ArithmeticError:
        return None

def solve_no_brackets(years, ops=('+', '-', '*', '/')):
    "All solutions to the countdown puzzle (with no brackets allowed) that match one of the years."
    exps = ('10'+a+'9'+b+'8'+c+'7'+d+'6'+e+'5'+f+'4'+g+'3'+h+'2'+i+'1'
            for a in ops    for b in ops    for c in ops 
            for d in ops    for e in ops    for f in ops 
            for g in ops    for h in ops    for i in ops)
    return {int(evaluate(exp)): exp
            for exp in exps if evaluate(exp) in years}

print solve_no_brackets(years=range(1900, 2100))
print

def countdown(start=10, end=1): return tuple(range(start, end-1, -1))

def splits(items):
    "Split sequence of items into two non-empty parts, in all ways."
    return [(items[:i], items[i:]) 
            for i in range(1, len(items))]

EXPS   = defaultdict(dict)    # e.g., EXPS[(10, 9, 8)][27]   == '((10+9)+8)'
COUNTS = defaultdict(Counter) # e.g., COUNTS[(10, 9, 8)][27] == 2

OPS = ('/', '*', '-', '+')

def fill_tables(numbers):
    "Fills EXPS and COUNTS tables for numbers, and all sub-sequences of numbers. Returns EXPS[numbers]"
    if numbers in EXPS: # Already did the work
        pass
    elif len(numbers) == 1: # Only one way to make an expression out of a single number
        table_entry(numbers, numbers[0], str(numbers[0]), 1)
    else: # Split in all possible ways, fill tables for left and right, then combine tables in all ways
        for (Lnums, Rnums) in splits(numbers):
            fill_tables(Lnums)
            fill_tables(Rnums)
            for (L, op, R) in itertools.product(EXPS[Lnums], OPS, EXPS[Rnums]):
                val = operate(L, op, R)
                if val is not None:
                    exp = '(' + EXPS[Lnums][L] + op + EXPS[Rnums][R] + ')'
                    table_entry(numbers, val, exp, COUNTS[Lnums][L] * COUNTS[Rnums][R])
    return EXPS[numbers]

def operate(L, op, R):
    "Return the numeric value of (L op R), or None on error."
    try:
        if op == '+': return L + R
        if op == '-': return L - R
        if op == '*': return L * R
        if op == '/': return L / R
    except ArithmeticError:
        return None

def table_entry(numbers, val, exp, count):
    "Record that there are count new ways to make val from numbers, and remember the shortest exp."
    COUNTS[numbers][val] += count
    if val not in EXPS[numbers] or len(exp) < len(EXPS[numbers][val]):
        EXPS[numbers][val] = exp

print fill_tables(countdown(10))[2016]

