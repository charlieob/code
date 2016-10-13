#include <stdio.h>
#include "types.h"

int f (const Bool_t);

void main ()
{
	int a;

	a = f (True_v);

	printf ("a=%d\n",a);
}
