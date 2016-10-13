#include <stdio.h>

main ()
{
int i=0;
unsigned int ui=0;
unsigned short us=0;

if ((unsigned short)(us+us) > us);
if (i+i > i);

printf ("%d\n", sizeof(us));
printf ("%d\n", sizeof(us+us));
}
