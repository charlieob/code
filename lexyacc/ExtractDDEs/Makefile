dde:		y.tab.c
		acc -g -DYYDEBUG -o dde y.tab.c

y.tab.c:	dde.y lex.yy.c
		ayacc -l -t dde.y

lex.yy.c:	dde.l
		lex dde.l
