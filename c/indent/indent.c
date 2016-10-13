#include <stdio.h>

#define INDENT_SPACING 3

static unsigned int indent=0;

void indentPlus ()
{
   indent++;
}

void indentMinus ()
{
   if (indent > 0)
      indent--;
}

void makeIndent (FILE *fp)
{
   fprintf (fp, "%*s", indent*INDENT_SPACING, "");
}

void newLine (FILE *fp)
{
   fputc ('\n', fp);
}

void openBlock (FILE *fp)
{
   makeIndent (fp);
   fprintf (fp, "{");
   newLine (fp);
   indentPlus ();
}

void closeBlock (FILE *fp)
{
   indentMinus ();
   makeIndent (fp);
   fprintf (fp, "}");
   newLine (fp);
}

void main ()
{
   openBlock (stdout);

   makeIndent (stdout);
   fprintf (stdout, "level 1");
   newLine (stdout);

   makeIndent (stdout);
   fprintf (stdout, "level 1");
   newLine (stdout);

   openBlock (stdout);

   makeIndent (stdout);
   fprintf (stdout, "level 2");
   newLine (stdout);

   openBlock (stdout);

   makeIndent (stdout);
   fprintf (stdout, "level 3");
   newLine (stdout);

   makeIndent (stdout);
   fprintf (stdout, "level 3");
   newLine (stdout);

   openBlock (stdout);

   makeIndent (stdout);
   fprintf (stdout, "level 4");
   newLine (stdout);

   closeBlock (stdout);

   makeIndent (stdout);
   fprintf (stdout, "level 3");
   newLine (stdout);

   openBlock (stdout);

   makeIndent (stdout);
   fprintf (stdout, "level 4");
   newLine (stdout);

   makeIndent (stdout);
   fprintf (stdout, "level 4");
   newLine (stdout);

   closeBlock (stdout);

   closeBlock (stdout);

   makeIndent (stdout);
   fprintf (stdout, "level 2");
   newLine (stdout);

   openBlock (stdout);

   makeIndent (stdout);
   fprintf (stdout, "level 3");
   newLine (stdout);

   closeBlock (stdout);

   closeBlock (stdout);

   makeIndent (stdout);
   fprintf (stdout, "level 1");
   newLine (stdout);

   closeBlock (stdout);
}
