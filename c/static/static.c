#include <stdio.h>

int f (void)
{
  static int n = 5;
  return (++n);
}

main ()
{
  printf ("%d\n", f());
}
