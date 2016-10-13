%%

^#.*				;

typedef				return TYPEDEF;
enum				return ENUM;
struct				return STRUCT;
union				return UNION;
unsigned			return UNSIGNED;
long				return LONG;
short				return SHORT;
char				return CHAR;
int				return INT;
extern				return EXTERN;
PACKED				return PACKED;

[A-Za-z][A-Za-z0-9_]*		|
[0-9]+[uUlL]?			|
0[xX][0-9a-fA-F]+[uUlL]?	{
				  yylval.string = (char *)
                                                  malloc (strlen(yytext) + 1);
				  strcpy (yylval.string, yytext);
				  return isalpha(yytext[0]) ?
							IDENTIFIER : INTEGER;
				}

[{}\[\](),;=*:]			return yytext[0];

[ \t\n]				;

%%
