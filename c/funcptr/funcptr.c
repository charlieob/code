#include <stdio.h>

int add (int a, int b)
{
  return a+b;
}

typedef int (*PFI)(int, int);

main ()
{
  PFI myfunc;

  myfunc = add;

  printf ("2+3=%d\n", (*myfunc)(2,3));
  printf ("4+5=%d\n", myfunc(4,5));
}

