#include "stdio.h"

#ifdef __STDC__
#define STRING "defined"
#else
#define STRING "not defined"
#endif

main ()
{
  printf ("the standard C name is %s\n", STRING);
}
