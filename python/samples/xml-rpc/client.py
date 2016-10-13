import xmlrpclib

s = xmlrpclib.ServerProxy('http://localhost:8000', verbose=True)
print s.pow(2,3)  # Returns 2**3 = 8
print s.add(2,3)  # Returns 5
# print s.add(2,3,4)  # too many arguments
print s.div(5,2)  # Returns 5//2 = 2
print s.sum_and_diff(7,4)

# Print list of available methods
print s.system.listMethods()

print s.system.methodSignature("div")
print s.system.methodHelp("pow")
