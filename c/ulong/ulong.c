#include <stdio.h>

unsigned long invokeId;

main ()
{
  int i;

  invokeId = 0xfffffff0;

  for (i=0; i<20; i++)
    printf ("%08x\n", invokeId++);

  printf ("\n");

  invokeId = 0xfffffff1;

  for (i=0; i<10; i++)
  {
    printf ("%08x\n", invokeId);
    invokeId += 2;
  }
}

