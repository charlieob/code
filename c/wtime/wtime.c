#include <stdio.h>
#include <sys/stat.h>

main
(
   int   argc,
   char *argv[]
)
{
   int i;
   struct stat stbuf;

   for (i=1; i<argc; i++)
   {
      if (stat(argv[i], &stbuf) != -1)
         printf ("%s %d\n", argv[i], stbuf.st_mtime);
   }
}

