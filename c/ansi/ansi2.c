#include "stdio.h"

typedef enum colour {red,green} colour;

/*
typedef unsigned long colour;
#define red 0L
#define green 1L
*/

void printColour (colour);

void printColour (colour c)
{
  printf ("colour=%d\n",c);
}

main()
{
  colour col;

  col = red;
  printColour (col);
  printColour (&col);
}
