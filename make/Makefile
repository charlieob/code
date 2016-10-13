Sample Makefile

Everyone always has trouble with their first makefile. So to make things easier, here's a sample makefile that you can tailor to suit your needs (though this should probably be sufficient for most small
applications). You can always check out the GNU Make manual, which is VERY thorough. 

To use this makefile, save your makefile under the name "makefile" or "Makefile". Next, change all the settings above the ##### RULES ##### line that need changing. Mostly you will just need to
change the SRC line and the OUT line, and the CC line if you want to use gcc instead of cc. And remember, if you want to make a smaller executable, you can always run "strip filename" to take all the
excess symbols from the file. Also, be sure to use TABS and not SPACES if you cut-and-paste this from your web browser, otherwise make will be really confused. You can get a copy of the entire
makefile here if you like. 

When you are all done, just type "make clean; make" and your program should build beautifully.

---

# Put all your source files here.
SRC=main.c source1.c source2.cpp
OBJ1=$(SRC:.c=.o)
OBJ=$(OBJ1:.cpp=.o)

# This is the name of your output file
OUT=runme

# This specifies all your include directories
INCLUDES=-I/usr/local/include -I.

# Put any flags you want to pass to the C compiler here.
CFLAGS=-g -O2 -Wall

# And put any C++ compiler flags here.
CCFLAGS=$(CFLAGS)

# CC speficies the name of the C compiler; CCC is the C++ compiler.
CC=cc
CCC=CC

# Put any libraries here.
LIBS=-L/usr/local/lib -lm
LDFLAGS=

##### RULES #####

# All rules are in the format:
#     item: [dependency list]
#     <tab>   command
# This means that "item" depends on what's in the dependency list; in other
# words, before "item" can be built, everything in the dependency list must
# be up to date.
# Note that this MUST be a tab, not a set of spaces!

.SUFFIXES: .c .c .ln

default: dep $(OUT)

.c.o: 
        $(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

.cpp.o: 
        $(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@


$(OUT): $(OBJ)
        $(CC) $(OBJ) $(LDFLAGS) $(LIBS) -o $(OUT)

depend: dep

dep:
        makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
        /bin/rm -f $(OBJ) $(OUT)