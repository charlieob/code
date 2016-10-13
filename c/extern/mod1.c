#include <stdio.h>

#ifdef __STDC__
void f(void);
#else
void f();
#endif

int n;

main()
{
  n=100;
  f();
}
