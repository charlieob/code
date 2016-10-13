#include <stdio.h>

unsigned char f (const unsigned char n)
{
	return n+1;
}

main ()
{
	unsigned char a,b;

	a = 10;
	b = f(a);

	printf ("b=%d\n", b);
}


