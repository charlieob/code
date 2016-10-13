#include <stdio.h>
#include <ieeefp.h>
#include <math.h>

#define mymacro(x) ((*(long *)&(x) & 0x7f800000L) == 0x7f800000L)

int main ()
{

	fp_rnd	r;
	float	f;

#if defined(__sparc__)
printf ("__sparc__ defined\n");
#endif

#if defined(__sparc)
printf ("__sparc defined\n");
#endif

#if defined(i386)
printf ("i386 defined\n");
#endif

#if !defined(i386)
printf ("i386 NOT defined\n");
#endif

#if defined(__ppc)
printf ("__ppc defined\n");
#endif

#if !defined(__ppc)
printf ("__ppc NOT defined\n");
#endif

#if defined(__STDC__)
printf ("__STDC__ defined\n");
#endif

	r = fpgetround();
	printf ("r=%x\n", (int)r);

	f = 0;
	printf ("isnan(f)=%d\n", isnan(f));
	printf ("isnanf(f)=%d\n", isnanf(f));
	printf ("isnanf(0)=%d\n", isnanf(0));

	printf ("FP_RN=%d\n", FP_RN);
	printf ("FP_RZ=%d\n", FP_RZ);
	printf ("FP_RP=%d\n", FP_RP);
	printf ("FP_RM=%d\n", FP_RM);

	printf ("FP_SNAN=%d\n", FP_SNAN);
	printf ("FP_QNAN=%d\n", FP_QNAN);
	printf ("FP_NINF=%d\n", FP_NINF);
	printf ("FP_PINF=%d\n", FP_PINF);
	printf ("FP_NDENORM=%d\n", FP_NDENORM);
	printf ("FP_PDENORM=%d\n", FP_PDENORM);
	printf ("FP_NZERO=%d\n", FP_NZERO);
	printf ("FP_PZERO=%d\n", FP_PZERO);
	printf ("FP_NNORM=%d\n", FP_NNORM);
	printf ("FP_PNORM=%d\n", FP_PNORM);

	printf ("FP_X_INV=%d\n", FP_X_INV);
	printf ("FP_X_OFL=%d\n", FP_X_OFL);
	printf ("FP_X_UFL=%d\n", FP_X_UFL);
	printf ("FP_X_DZ=%d\n", FP_X_DZ);
	printf ("FP_X_IMP=%d\n", FP_X_IMP);

	printf ("mymacro(f)=%d\n", mymacro(f));
	printf ("&(f)=%x\n", &(f));
	printf ("(long *)&(f)=%x\n", (long *)&(f));
	printf ("*(long *)&(f)=%x\n", *(long *)&(f));
//	printf ("mymacro(0)=%d\n", mymacro(0));

/*
-- The following comparison (NaN == float) should be false (i.e. unordered).
*/
    printf ("\n\nTesting a comparison ...\n");

    f = (float) sqrt(-1);
    printf ("The following condition should be false (0): %d\n", f == 1.5f);

}
