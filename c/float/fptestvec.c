#include <stdio.h>
#include <ieeefp.h>
#include <signal.h>

void fpexception (int sig)
/*
-- The parameter sig gets the value of the signal that has
-- been raised (SIGFPE=8 in this case).
--
-- Not really sure what you would want to do in an exception handler for
-- a floating point exception.
*/
{
	signal (SIGFPE, fpexception);
	printf ("fpexception(%d): fpgetsticky()=%d\n", sig, fpgetsticky());
}

int test_add (char *filename)
{
	FILE*	fp;
	char	line[200];

	char	opcode[10], dummy[10], exceptions[10];
	char	rnd;
	char	opnd1s[9], opnd2s[9], ress[9];

	char	finished=0;

	char	data[4];
	char	datasum[4];

	unsigned long
		*pi, *pisum;
	float	*pf;

	float	opnd1, opnd2, sum;
	unsigned long
		opnd1i, opnd2i, resi;

	fp_rnd	rnd_dir;
	int	sticky, st;

	int	tests=0, passed=0, failed=0;
	char	nanresult;

/*
-- This will cause the exception handler to start if
-- an arithmetic exception occurs in a FP operation.
--
-- I think the FP operation is then incomplete, which
-- is not the case if the exception is ignored.
*/

	if (signal (SIGFPE, SIG_IGN) != SIG_IGN)
		signal (SIGFPE, fpexception);

/*
-- The FP exception mask is 0 default.
-- Set it here if we want to trigger any exceptions.
*/

	// fpsetmask (FP_X_INV | FP_X_OFL);

	pi = (unsigned long *) data;
	pf = (float *) data;
	pisum = (unsigned long *) datasum;

	fp = fopen (filename, "r");

	while (!finished)
	{
		if (fgets (line, sizeof(line), fp) == NULL)
			finished = 1;

		if (!finished && (line[0] != '#'))
		{
			sscanf (line, "%s %c %s %s %s %s %s %s",
				opcode, &rnd, dummy, exceptions,
				opnd1s, opnd2s, ress);

			sticky = 0;
			if (exceptions[0] != '-')
			{
				if (strchr(exceptions, 'v'))
					sticky |= FP_X_INV;

				if (strchr(exceptions, 'o'))
					sticky |= FP_X_OFL;

				if (strchr(exceptions, 'u'))
					sticky |= FP_X_UFL;

				if (strchr(exceptions, 'd'))
					sticky |= FP_X_DZ;

				if (strchr(exceptions, 'x'))
					sticky |= FP_X_IMP;
			}

			*pi = strtoul (opnd1s, (char**)NULL, 16);
			opnd1i = *pi;
			opnd1  = *pf;

			*pi = strtoul (opnd2s, (char**)NULL, 16);
			opnd2i = *pi;
			opnd2  = *pf;

			*pi  = strtoul (ress, (char**)NULL, 16);
			resi = *pi;
			nanresult = isnanf (*pf);

			switch (rnd)
			{
			case 'z':	rnd_dir = FP_RZ;
					break;
			case 'p':	rnd_dir = FP_RP;
					break;
			case 'm':	rnd_dir = FP_RM;
					break;
			case 'n':
			default:	rnd_dir = FP_RN;
					break;
			}

			fpsetsticky (0);
			fpsetround (rnd_dir);
			sum = opnd1 + opnd2;
			st = fpgetsticky();

			*pf = sum;

			tests++;

			if ((resi == *pi) && (sticky == st))
				passed++;
			else
			{
				if (nanresult && isnanf (sum))
				{
					passed++;
				}
				else
				{
					failed++;
					printf ("{%c,%s} opnd1=%08x,opnd2=%08x,exp=%08x,act=%08x\n",
						rnd, exceptions, opnd1i, opnd2i, resi, *pi);
					printf ("sticky=%d,%d\n", sticky, st);
				}
			}

		}
	}

	fclose (fp);

	printf ("tests: %d, passed=%d, failed=%d\n", tests, passed, failed);

	return 0;
}

main ()
{
	test_add ("adds.dat");
}
