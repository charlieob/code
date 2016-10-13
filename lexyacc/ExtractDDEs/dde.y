%{
#include <stdio.h>
#include <string.h>

typedef enum
{
	lit,	/* literal */
	cat,	/* concatenation */
	sel,	/* selection */
	iter,	/* iteration */
	mod,	/* modifier */
	ptr,	/* pointer */
	qual	/* qualifier */
} NodeLabel;

typedef struct Node
{
	NodeLabel	label;
	struct Node	*n1;
	struct Node	*n2;
	char		*lit;
} Node;

Node	*n;
char	*str;
FILE	*outfile;
	
%}

%union
{
	char*	string;
	Node*	node;
}

%token	TYPEDEF ENUM STRUCT UNION
%token	UNSIGNED LONG SHORT CHAR INT
%token	EXTERN PACKED

%token	<string>	IDENTIFIER INTEGER

%type	<node>		type structType unionType enumType simpleType
%type	<node>		identifier integer
%type	<node>		structBody unionBody enumBody
%type	<node>		structElement enumElement enumerator

%start	declarations

%%

declarations	:	/* empty */
		|	declarations declaration
		;

declaration	:	typedefDecl
		|	funcDecl
		;

typedefDecl	:	TYPEDEF type IDENTIFIER ';'
			{
			  printf ("%s\n", $3);
			  outfile = fopen ($3, "w");
			  printNode (outfile, $2, -1, lit);
			  fprintf (outfile, "\n");
			}
		;

type		:	structType
			{
			  $$ = $1;
			}
		|	PACKED structType
			{
			  $$ = $2;
			}
		|	unionType
			{
			  $$ = $1;
			}
		|	enumType
			{
			  $$ = $1;
			}
		|	simpleType
			{
			  $$ = $1;
			}
		;

structType	:	STRUCT '{' structBody '}'
			{
			  $$ = $3;
			}
		|	STRUCT IDENTIFIER '{' structBody '}'
			{
			  $$ = $4;
			}
		;

unionType	:	UNION '{' unionBody '}'
			{
			  $$ = $3;
			}
		|	UNION IDENTIFIER '{' unionBody '}'
			{
			  $$ = $4;
			}
		;

enumType	:	ENUM '{' enumBody '}'
			{
			  $$ = $3;
			}
		|	ENUM IDENTIFIER '{' enumBody '}'
			{
			  $$ = $4;
			}
		;

simpleType	:	UNSIGNED LONG
			{
			  $$ = makeNode (lit, NULL, NULL, "unsigned_long");
			}
		|	UNSIGNED SHORT
			{
			  $$ = makeNode (lit, NULL, NULL, "unsigned_short");
			}
		|	UNSIGNED CHAR
			{
			  $$ = makeNode (lit, NULL, NULL, "unsigned_char");
			}
		|	UNSIGNED INT
			{
			  $$ = makeNode (lit, NULL, NULL, "unsigned_int");
			}
		|	SHORT
			{
			  $$ = makeNode (lit, NULL, NULL, "short");
			}
		|	identifier
			{
			  $$ = $1;
			}
		;

identifier	:	IDENTIFIER
			{
			  $$ = makeNode (lit, NULL, NULL, $1);
			}
		;

integer		:	INTEGER
			{
			  $$ = makeNode (lit, NULL, NULL, $1);
			}
		;

structBody	:	structElement
			{
			  $$ = $1;
			}
		|	structBody structElement
			{
			  $$ = makeNode (cat, $1, $2, NULL);
			}
		;

structElement	:	type identifier ';'
			{
			  $$ = makeNode (mod, $2, $1, NULL);
			}
		|	type '*' identifier ';'
			{
			  n = makeNode (ptr, $1, NULL, NULL);
			  $$ = makeNode (mod, $3, n, NULL);
			}
		|	type identifier '[' identifier ']' ';'
			{
			  n = makeNode (iter, $1, $4, NULL);
			  $$ = makeNode (mod, $2, n, NULL);
			}
		|	type identifier ':' integer ';'
			{
			  n = makeNode (mod, $2, $1, NULL);
			  $$ = makeNode (qual, n, $4, NULL);
			}
		;

unionBody	:	structElement
			{
			  $$ = $1;
			}
		|	unionBody structElement
			{
			  $$ = makeNode (sel, $1, $2, NULL);
			}
		;

enumBody	:	enumElement
			{
			  $$ = $1;
			}
		|	enumBody ',' enumElement
			{
			  $$ = makeNode (sel, $1, $3, NULL);
			}
		;

enumElement	:	enumerator
			{
			  $$ = $1;
			}
		|	enumerator '=' integer
			{
			  $$ = makeNode (qual, $1, $3, NULL);
			}
		|	enumerator '=' identifier
			{
			  $$ = makeNode (qual, $1, $3, NULL);
			}
		;

enumerator	:	IDENTIFIER
			{
			  str = (char *) malloc (strlen($1) + 3);
			  sprintf (str, "\"%s\"", $1);
			  free ($1);
			  $$ = makeNode (lit, NULL, NULL, str);
			}
		;

funcDecl	:	EXTERN funcDecl
		|	simpleType IDENTIFIER '(' argList ')' ';'
		|	simpleType IDENTIFIER '(' ')' ';'
		;

argList		:	argument
		|	argList ',' argument
		;

argument	:	simpleType
		|	simpleType '*'
		|	simpleType IDENTIFIER
		|	simpleType '*' IDENTIFIER
		;

%%

#include "lex.yy.c"

int	main (void)
{
	return yyparse();
}

void	yyerror
(
	const char *s
)
{
	fprintf (stderr, "%s\n", s);
}

int	yywrap (void)
{
	return 1;
}

Node*	makeNode
(
	NodeLabel	label,
	Node		*n1,
	Node		*n2,
	char		*lit
)
{
	Node		*n;

	n = (Node *) malloc (sizeof(Node));
	n->label = label;
	n->n1    = n1;
	n->n2    = n2;
	n->lit   = lit;

	return n;
}

void	printIndent
(
	FILE		*fp,
	int		indent
)
{
	int		i;

	fprintf (fp, "\n");
	for (i=0; i<indent; i++)
		fprintf (fp, "  ");
}

void	printNode
(
	FILE		*fp,
	Node		*n,
	int		indent,
	NodeLabel	parent
)
{
	switch (n->label)
	{

	case lit:
		fprintf (fp, "%s", n->lit);
		break;

	case cat:
		if (parent != cat)
			indent++;

		printNode (fp, n->n1, indent, cat);
		printIndent (fp, indent);
		fprintf (fp, "+ ");
		printNode (fp, n->n2, indent, cat);

		break;

	case sel:
		if (parent != sel)
		{
			indent++;
			fprintf (fp, "[ ");
		}

		printNode (fp, n->n1, indent, sel);
		printIndent (fp, indent);
		fprintf (fp, "| ");
		printNode (fp, n->n2, indent, sel);

		if (parent != sel)
		{
			printIndent (fp, indent);
			fprintf (fp, "]");
		}

		break;

	case iter:
		fprintf (fp, "{");
		printNode (fp, n->n1, indent, iter);
		fprintf (fp, "}");
		printNode (fp, n->n2, indent, iter);
		break;

	case mod:

		if (n->n2->label == lit)
		{
			fprintf (fp, "<");
			printNode (fp, n->n1, indent, mod);
			fprintf (fp, ">");
			printNode (fp, n->n2, indent, mod);
		}
		else
			printNode (fp, n->n2, indent, mod);
		break;

	case ptr:
		fprintf (fp, "-> ");
		printNode (fp, n->n1, indent, ptr);
		break;

	case qual:

		fprintf (fp, "*");
		printNode (fp, n->n2, indent, qual);
		fprintf (fp, "* ");
		printNode (fp, n->n1, indent, qual);
		break;

	}
}
