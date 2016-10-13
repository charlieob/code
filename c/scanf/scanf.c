#include <stdio.h>

main()
{
	char str[80] = "c 1 2 3";
	char ch;
	int p, q, r, s;
	
	sscanf(str, "%c %d %d", &ch, &p, &q);
	printf("ch= %c, p=%d, q=%d, r=%d, s=%d\n", ch, p, q, r, s);
}
