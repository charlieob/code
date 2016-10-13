#include <stdio.h>

main()
{
  int n=2;

  switch (n)
  {

/* it is not possible to define variables inside a switch statement
   like this (the code is unreachable).
*/
  int m=0;

  case 1:

/* the compiler also won't allow this */
    int m=0;

    printf ("m=%d,n=%d\n", m,n);
    break;

  case 2:

/* the only way to do it is like this, which unambiguously specifies the
   scope of the variable:
*/
  {
    int m=0;

    printf ("m=%d,n=%d\n", m,n);
  }
    break;
  }
}
