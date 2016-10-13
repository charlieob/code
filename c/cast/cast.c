#include <stdio.h>

main()
{
  int i;
  short s;

  s = -60;
  i = s;
  printf ("short : %d (%0hX)\n", s, s);
  printf ("int   : %d (%0X)\n", i, i);
}
