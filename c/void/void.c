#include <stdio.h>

main()
{
  unsigned char c,d,e;
  unsigned char *p;
  void *v;

  c = 1;
  p = &c;
  v = (void *) p;

  d = (char *) v;
  e = *((char *) v);

  printf ("d=%d\n",d);
  printf ("e=%d\n",e);
}
