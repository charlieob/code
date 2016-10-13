#include <stdio.h>

#define	YARDS	1.094
#define	METRES	0.914

main ()
{
	int i;
	int m;
	int y;

	printf ("metres  yards          yards  metres\n");

	for (i=8; i<=22; i++)
	{
		m = 10*i;
		y = (int) ((m*YARDS) + 0.5);
		printf ("%6d  %5d", m, y);

		printf ("          ");

		y = 10*i;
		m = (int) ((y*METRES) + 0.5);
		printf ("%5d  %6d\n", y, m);
	}
}
