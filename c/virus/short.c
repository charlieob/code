#include <stdio.h>
char str[]="main()\n{\nint i;\nprintf(\"#include <stdio.h>\\n\");\nprintf(\"char str[]=\\\"\");\nfor (i=0; i<strlen(str); i++)\n switch (str[i]) {\n case \'\\n\': printf(\"\\\\n\"); break;\n case \'\\\"\': printf(\"\\\\\\\"\"); break;\n case \'\\\'\': printf(\"\\\\\\\'\"); break;\n case \'\\\\\': printf(\"\\\\\\\\\"); break;\n default: putchar(str[i]); break;\n }\nprintf(\"\\\";\\n\");\nfor (i=0; i<strlen(str); i++)\n putchar(str[i]);\n}\n";
main()
{
int i;
printf("#include <stdio.h>\n");
printf("char str[]=\"");
for (i=0; i<strlen(str); i++)
 switch (str[i]) {
 case '\n': printf("\\n"); break;
 case '\"': printf("\\\""); break;
 case '\'': printf("\\\'"); break;
 case '\\': printf("\\\\"); break;
 default: putchar(str[i]); break;
 }
printf("\";\n");
for (i=0; i<strlen(str); i++)
 putchar(str[i]);
}
