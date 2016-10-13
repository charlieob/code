#include <stdio.h>
#include <limits.h>

#include "mp.h"

/*
The following macros only work for 1<=m<=N, where N is the
number of bits in an int. This is because the result of the
shift operators is undefined if the right operand is negative
or >= the number of bits in the left operand.
*/

#define LSB_0(m)	((~0x1)<<(m-1))
#define LSB_1(m)	(~((~0x1)<<(m-1)))

int	digit2val
(
	char	c,
	int	radix
)
{
	int	d;

	switch (c)
	{
	case '0':	d = 0;  break;
	case '1':	d = 1;  break;
	case '2':	d = 2;  break;
	case '3':	d = 3;  break;
	case '4':	d = 4;  break;
	case '5':	d = 5;  break;
	case '6':	d = 6;  break;
	case '7':	d = 7;  break;
	case '8':	d = 8;  break;
	case '9':	d = 9;  break;
	case 'A':
	case 'a':	d = 10; break;
	case 'B':
	case 'b':	d = 11; break;
	case 'C':
	case 'c':	d = 12; break;
	case 'D':
	case 'd':	d = 13; break;
	case 'E':
	case 'e':	d = 14; break;
	case 'F':
	case 'f':	d = 15; break;
	default:	return -1;
	}

	if (d < radix)
		return d;
	else
		return -1;
}

mpnum	*mpNew
(
	int	size,   /* bits */
	int	val
)
{
	int	i;

	mpnum*	n = (mpnum *) malloc(sizeof(mpnum));

	n->len = 1 + size/MPCHUNKSIZE; 
	n->val = (mpchunk *) calloc(n->len, MPCHUNKSIZE); 

	n->val[0] = val;

	for (i=1; i<n->len; i++)
		if (val < 0) n->val[i] = ~0;

	return n;
}

void	mpFree
(
	mpnum	*n
)
{
	free (n->val);
	free (n);
}

void	mp2Hex
(
	char	*s,
	mpnum	*n,
	int	word
)
{
	char	t[100];
	int	a;
	int	i;
	int	loc, bits;

	strcpy (s, "");

	while (word > 0)
	{
		i = (word-1) / MPCHUNKSIZE;
		loc = (word-1) % MPCHUNKSIZE;
		bits = (loc % 4) + 1;

		a = n->val[i] >> (4 * (loc / 4));
		a &= LSB_1(bits);

		sprintf (t, "%x", a);
		strcat (s, t);

		word -= bits;
	}
}

void	mp2Decimal
(
	char	*s,
	mpnum	*n,
	int	word
)
{
/*
write this function
*/
}

/*
Should also add a function hex2mp() to allow input of
generalised hex strings.
*/

int	string2mp
(
	mpnum	*n,
	int	word,
	int	frac,
	char	*str,
	int	radix
)
{
	int	minus;
	int	digit;
	int	fracDigits[100];
	int	fracLen;
	int	fracVal;
	int	oldN;

	if ((radix < 2) || (radix > 16))
		return 3;

/*
Note: must have at least one bit before the fixed point, for the sign
*/
	if ((word < 1) || (frac < 0) || (frac >= word))
		return 4;

	if (word > MPCHUNKSIZE)
		return 5;

	n->val[0] = 0;

	if (*str == '-')
	{
		minus = 1;
		str++;
	}
	else
		minus = 0;

/*
Overflow can occur in the next step, due to the single precision arithmetic.
For example, converting the string 1000000ab in radix 16 with wordsize 32,0 is
an overflow which will not be detected.
*/

	while ((digit = digit2val(*str, radix)) != -1)
	{
		n->val[0] = n->val[0] * radix + digit;
		str++;
	}

	if (minus)
	{
		if (n->val[0] > (0x1 << (word-frac-1)))
		{
			/* overflow */
			return 2;
		}

		n->val[0] = -(n->val[0]);
	}
	else
	{
		if (n->val[0] > (0x1 << (word-frac-1)) - 1)
		{
			/* overflow */
			return 2;
		}
	}

	n->val[0] = n->val[0] << frac;

	if (*str != '\0')
	{

		if (*str != '.')
		{
			/* unexpected character */
			return 1;
		}

		str++;

		fracLen = 0;
		while ((digit = digit2val(*str, radix)) != -1)
		{
			fracDigits[fracLen++] = digit;
			str++;
		}

		if (*str != '\0')
		{
			/* unexpected character */
			return 1;
		}

		fracVal = 0;
		for (; fracLen > 0; fracLen--)
		{
			fracVal = (fracVal + (fracDigits[fracLen-1] * (0x1<<(frac)))) / radix;
		}

/*
The fractional part will be truncated rather than rounded.
Rounding can be achieved by using frac+1 instead of frac in the left-shift of 0x1
in the line above, and then using the 2 lines below.
However, rounding of the fractional part can round over the decimal point, which
in turn can round the integer part past the overflow limit and affext the sign.
So it's tricky.

For example: 1.9921875 with wordsize 8,6 should round up to 2.0 which is an overflow.

		fracVal += 1;
		fracVal = fracVal >> 1;
*/

		if (minus)
			n->val[0] -= fracVal;
		else
			n->val[0] += fracVal;
	}

	return 0;
}

//#define TESTMAIN 1

#ifdef TESTMAIN

int verbose = 0;

char	*testData[] =
{
	"10 3 6 2",		"0c",
	"10 1.5 6 4",		"18",

	"2 10.01101 7 5",	"overflow",
	"2 010.01101 8 5",	"4d",

	"10 1.5 8 6",		"60",
	"10 2 8 6",		"overflow",
	"10 -1.5 8 6",		"a0",
	"10 -2 8 6",		"80",
	"10 -2 7 5",		"40",
	"10 -3 8 6",		"overflow",
	"10 -3 7 5",		"overflow",
	"10 1.5 8 6",		"60",
	"10 1.75 8 6",		"70",
	"10 1.875 8 6",		"78",
	"10 1.9375 8 6",	"7c",
	"10 1.96875 8 6",	"7e",
	"10 1.984375 8 6",	"7f",
	"10 1.9921874 8 6",	"7f",
	"10 1.9921875 8 6",	"7f",
	"10 1.5 6 4",		"18",
	"10 1.49 6 4",		"17",
	"10 1.51 6 4",		"18",
	"10 1.5624 6 4",	"18",
	"10 1.5625 6 4",	"19",
	"10 1.53124 6 4",	"18",
	"10 1.53125 6 4",	"18",

	"16 4b 8 0",		"4b",
	"16 12345678 32 0",	"12345678",
	"16 123456.78 32 8",	"12345678",
	"16 12345678 40 4",	"wordsize too big",
	"16 89abcdef 32 0",     "overflow",

	"2 1011.01 5 2",	"overflow",

	"10 1 32 32",		"bad wordsize",

	"2 10001001101010111100110111101111 32 4",
				"overflow",
	"16 89abcdef 32 4",	"overflow",
	"16 12345678 32 4",	"overflow",
	"16 89abcde 32 4",	"overflow",
	"16 1234567 32 4",	"12345670",
	"16 -1234567 32 4",	"edcba990",
	"2 0.10001001101010111100110111101111 32 28",
				"089abcde",
};

int	testLen = sizeof(testData)/sizeof(*testData)/2;

int	main ()
{
	char	str[100];
	char	res[100];
	mpnum	*n;
	int	radix;
	int	word;
	int	frac;
	int	i;
	int	failures=0;
	int	status;

/* here's a test for the MSB:
unsigned int t, tt;
t = 0;
tt = t << 1;
tt = tt >> 1;
if (tt != t) printf ("MSB is 1\n"); else printf ("MSB is 0\n");

t = ~t;
tt = t << 1;
tt = tt >> 1;
if (tt != t) printf ("MSB is 1\n"); else printf ("MSB is 0\n");
*/


	for (i=0; i<testLen; i++)
	{
		sscanf (testData[i*2], "%d %s %d %d", &radix, str, &word, &frac);

		n = mpNew (word, 0);
		status = string2mp (n, word, frac, str, radix);

		switch (status) {
		case 1:		strcpy (res, "bad string");
				break;
		case 2:		strcpy (res, "overflow");
				break;
		case 3:		strcpy (res, "bad radix");
				break;
		case 4:		strcpy (res, "bad wordsize");
				break;
		case 5:		strcpy (res, "wordsize too big");
				break;

		default:	mp2Hex (res, n, word);
		}

		mpFree (n);

		if (verbose)
		{
			fprintf (stdout, "test case %d\n", i);
			fprintf (stdout, "input:    %d %d %s %d\n", word, frac, str, radix);
			fprintf (stdout, "output:   %s\n", res);
			fprintf (stdout, "expected: %s\n", testData[i*2+1]);
		}

		if (strcmp(res,testData[i*2+1]))
		{
			failures++;

			if (verbose)
				fprintf (stdout, "TEST CASE FAILED\n");
		}

		if (verbose)
			fprintf (stdout, "------\n");
	}

	fprintf (stdout, "tests passed: %d\n", testLen-failures);
	fprintf (stdout, "tests failed: %d\n", failures);

	if (failures > 0)
		fprintf (stdout, "TEST FAILED: %d failures occurred\n", failures);
	else
		fprintf (stdout, "TEST PASSED\n");
}

#endif
