%{
#include <stdio.h>
%}

%s comment

%%


<INITIAL>"//"[^\n]*		;
<INITIAL>"/*"			BEGIN comment;
<comment>"*/"			BEGIN INITIAL;
<comment>.			;

%%

int yywrap()
{
	return 1;
}

main (int argc, char *argv[])
{
	argv++;
	argc--;

	if (argc > 0)
		yyin = fopen (argv[0], "r");
	else
		yyin = stdin;

	yylex();
}
