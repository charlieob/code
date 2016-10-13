my_list = ['john', 'pat', 'gary', 'michael']
print enumerate(my_list)
print list(enumerate(my_list))
for i, name in enumerate(my_list):
    print "iteration %i is %s" % (i, name)
