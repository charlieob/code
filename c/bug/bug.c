#include <stdio.h>

typedef struct list_t
{
	int	x;
	int	y;
	int	z;
} list_t;

int myfunc (unsigned char, list_t [], unsigned short *);

int	myfunc
(
	unsigned char	a,
	list_t		b[],
	unsigned short	*c
)
{
	b[1].x = a+1;
	*c = b[1].x;
}

main ()
{
	list_t		a[10];
	unsigned short	uc;
	unsigned short	us;

	uc = 0;
	a[0].x = 10;
	a[1].x = 11;

	myfunc (uc, a, &us);

	printf ("us = %d\n", us);
}
