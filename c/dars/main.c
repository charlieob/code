#include <stdio.h>

#include "dar.h"

int protectedData;

main ()
{
  setProtectedData(34);
  protectedData = 0;
  printf("protected data = %d\n", getProtectedData());
}
