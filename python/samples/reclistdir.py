import os

def recls(path, indent=0):
	ls = [el for el in os.listdir(path) if el[0] <> '.']
	dirs = [d for d in ls if os.path.isdir(os.path.join(path, d))]
	for i in sorted(dirs):
		print i.rjust(len(i) + indent*4)
		recls(os.path.join(path, i), indent+1)

if __name__ == "__main__":
	print "running as main"
	print(os.path.expanduser("~"))
	recls(os.path.expanduser("~"))
	#recls(os.curdir)

