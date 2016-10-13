#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <ieeefp.h>

int	main ()
{
	char	d[4];
	float	*pf;
	int	*pi;

	printf ("FLT_RADIX=%d\n", FLT_RADIX);
	printf ("FLT_DIG=%d\n", FLT_DIG);
	printf ("FLT_EPSILON=%e\n", FLT_EPSILON);
	printf ("FLT_MANT_DIG=%d\n", FLT_MANT_DIG);
	printf ("FLT_MAX=%e\n", FLT_MAX);
	printf ("FLT_MAX_EXP=%d\n", FLT_MAX_EXP);
	printf ("FLT_MIN=%e\n", FLT_MIN);
	printf ("FLT_MIN_EXP=%d\n", FLT_MIN_EXP);
	printf ("bits in float = %d\n", CHAR_BIT*sizeof(float));
	printf ("bits in int = %d\n", CHAR_BIT*sizeof(int));
	printf ("bits in long = %d\n", CHAR_BIT*sizeof(long));
	printf ("bits in long long = %d\n", CHAR_BIT*sizeof(long long));

	pf = (float*) d;
	pi = (int*) d;

	*pf = 0;
	printf ("\n");
	printf ("data(float) = %f\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

	*pf = FLT_MAX;
	printf ("\n");
	printf ("data(float) = %e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

	*pf = FLT_MIN;
	printf ("\n");
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

	*pf = 1;
	printf ("\n");
	printf ("data(float) = %0.2f\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

	*pf = -1;
	printf ("\n");
	printf ("data(float) = %0.2f\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

	*pf = 2;
	printf ("\n");
	printf ("data(float) = %0.2f\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

	*pf = 4;
	printf ("\n");
	printf ("data(float) = %0.2f\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

	*pf = 8;
	printf ("\n");
	printf ("data(float) = %0.2f\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

	*pf = 16;
	printf ("\n");
	printf ("data(float) = %0.2f\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

	*pf = 32;
	printf ("\n");
	printf ("data(float) = %0.2f\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

	*pf = 33;
	printf ("\n");
	printf ("data(float) = %0.2f\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

	*pf = 1 << 16;
	printf ("\n");
	printf ("data(float) = %0.2f\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

	*pf = 1 << 17;
	printf ("\n");
	printf ("data(float) = %0.2f\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

/* smallest normalised */

	*pi = 0x00800000;
	printf ("\n");
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);
	printf ("dividing int by 2 ...\n");
	*pi = *pi / 2;
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

/* smallest denormalised */

	*pi = 0x00000001;
	printf ("\n");
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);
	printf ("fpclass     = %d (should be 4 or 5 for a denorm)\n", fpclass(*pf));
	printf ("dividing float by 2 ...\n");
	*pf = *pf/2;
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

/* largest positive */

	*pi = 0x7f7fffff;
	printf ("\n");
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);
	printf ("adding 1%% to float ...\n");
	*pf = *pf * 1.01;
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

/* largest negative */

	*pi = 0xff7fffff;
	printf ("\n");
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

/* zero */
	*pi = 0x00000000;
	printf ("\n");
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

/* minus zero */

	*pi = 0x80000000;
	printf ("\n");
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

/* infinity */

	*pi = 0x7f800000;
	printf ("\n");
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

/* minus infinity  */

	*pi = 0xff800000;
	printf ("\n");
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);
	printf ("multiplying float by -1 ...\n");
	*pf = *pf * (-1);
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);
	printf ("dividing float by 256 ...\n");
	*pf = *pf / 256;
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);
	printf ("dividing float by -1000 ...\n");
	*pf = *pf / (-1000);
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

/* Not a Number (NaN) */

	*pi = 0x7f801234;
	printf ("\n");
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);
	printf ("fpclass     = %d\n", fpclass (*pf));

	*pi = 0xff898765;
	printf ("\n");
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);
	printf ("fpclass     = %d\n", fpclass (*pf));

	*pi = 0x7fffffff;
	printf ("\n");
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);
	printf ("fpclass     = %d\n", fpclass (*pf));

	*pi = 0x7fbfffff;
	printf ("\n");
	printf ("data(float) = %0.2e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);
	printf ("fpclass     = %d (1 means signalling NaN)\n", fpclass (*pf));

/* specific values */
	*pi = 0x00100000; // note: this is a denorm
	printf ("\n");
	printf ("data(float) = %0.8e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

	*pi = 0x00600000;
	printf ("\n");
	printf ("data(float) = %0.8e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

	*pi = 0x00700000;
	printf ("\n");
	printf ("data(float) = %0.8e\n", *pf);
	printf ("data(int)   = %08x\n", *pi);

}
