#include <stdio.h>

typedef struct type1
{
	struct type2 *a;
} type1;

main ()
{
	type1	t1;
	struct type2 *b;
	int	i=20;

	t1.a = (struct type2 *)&i;

	printf ("%d\n",*((int*)(t1.a)));
}
