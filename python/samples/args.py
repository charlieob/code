def test_var_args(farg, *args):
    print "formal arg:", farg
    for arg in args:
        print "another arg:", arg
    print args
    for arg in args:
        print "another arg:", arg

test_var_args(1, "two", 3)
