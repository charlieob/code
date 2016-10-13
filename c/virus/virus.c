#include <stdio.h>

/*
-- data string
*/
char str[]="\
/*\n\
-- program starts here\n\
*/\n\
main()\n\
{\n\
 int i;\n\
 printf(\"#include <stdio.h>\\n\\n/*\\n-- data string\\n*/\\n\");\n\
 printf(\"char str[]=\\\"\\\\\\n\");\n\
 for (i=0; i<strlen(str); i++)\n\
  switch (str[i])\n\
  {\n\
   case \'\\n\': printf(\"\\\\n\\\\\\n\"); break;\n\
   case \'\\\"\': printf(\"\\\\\\\"\"); break;\n\
   case \'\\\'\': printf(\"\\\\\\\'\"); break;\n\
   case \'\\\\\': printf(\"\\\\\\\\\"); break;\n\
   default: putchar(str[i]); break;\n\
  }\n\
 printf(\"\\\";\\n\\n\");\n\
 for (i=0; i<strlen(str); i++)\n\
  putchar(str[i]);\n\
}\n\
";

/*
-- program starts here
*/
main()
{
 int i;
 printf("#include <stdio.h>\n\n/*\n-- data string\n*/\n");
 printf("char str[]=\"\\\n");
 for (i=0; i<strlen(str); i++)
  switch (str[i])
  {
   case '\n': printf("\\n\\\n"); break;
   case '\"': printf("\\\""); break;
   case '\'': printf("\\\'"); break;
   case '\\': printf("\\\\"); break;
   default: putchar(str[i]); break;
  }
 printf("\";\n\n");
 for (i=0; i<strlen(str); i++)
  putchar(str[i]);
}
