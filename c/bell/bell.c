#include <stdio.h>

#define BELL 0x07

main(int argc, char *argv[])
{
  char ch=BELL;
  int n,i;

  if (argc == 1)
  {
    putc (ch, stdout);
  }
  else if (argc == 2)
  {
    n = atoi (argv[1]);
    for (i=0; i<n; i++)
      putc (ch, stdout);
  }
  else
  {
    fprintf (stderr, "bell: too many arguments\n");
    fprintf (stderr, "Usage: bell [n]\n");
  }
}
