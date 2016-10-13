#include <stdio.h>

main ()
{
	unsigned char ch;

	ch=0;
	do	printf("%6d%6x%6c\n",ch,ch,ch),ch++;
	while (ch != 0);
}
