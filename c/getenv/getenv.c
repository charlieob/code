#include <stdio.h>
#include <stdlib.h>

main ()
{
   char *s;

   s = getenv ("STRING");

   if (s != NULL)
      printf ("STRING = %s\n", s);
   else
      printf ("STRING not defined\n");
}
