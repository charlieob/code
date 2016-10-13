#include <stdio.h>

extern void myFunc (unsigned short);

void myFunc (unsigned short n)
{
	printf ("%d\n", n);
}

main ()
{
	long l;

	l = 10;

	myFunc (l);
	myFunc (20);
}
