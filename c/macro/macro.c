#include <stdio.h>

#define gm_xxph_CreateIdentification( X , Y )  (((unsigned long) (X) ) << 16) | ((unsigned long)(Y))

main ()
{
  unsigned long n;

  n = gm_xxph_CreateIdentification(0x00,'EFGH');
  printf("n=%x\n",n);
}
