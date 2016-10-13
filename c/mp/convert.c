#include <stdio.h>

typedef unsigned int uint;

/*
Warning, the LSB_ macros will not work if m=32, i.e. the number of
bits in an integer, because the shift operators have an undefined result
when the right operand is equal to the number of bits in the left operand.
*/
#define	GET_BYTE(m,n)	(((m)>>(8*(n)))&0xff)
#define GET_BIT(m,n)	(((m)>>(n))&0x1)
#define LSB_0(m)	((~0x0)<<(m))
#define LSB_1(m)	(~((~0x0)<<(m)))

int	convertDigit
(
	char	c,
	uint	radix
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

void	truncate
(
	int	*n,
	int	word
)
{
	int	minus;

	minus = GET_BIT(*n,word-1);
	if (minus)
		*n = *n | LSB_0(word);
	else
		*n = *n & LSB_1(word);
}

int	convert
(
	uint	*n,
	uint	word,
	uint	frac,
	char	*str,
	uint	radix
)
{
	char	*cp;
	int	minus;
	int	digit;
	int	fracDigits[100];
	int	fracLen;
	int	fracVal;
	int	oldN;

	*n = 0;
	cp = str;

	minus = 0;
	if (*cp == '-')
	{
		minus = 1;
		cp++;
	}

	while ((digit = convertDigit(*cp, radix)) != -1)
	{
		*n = *n * radix + digit;
		cp++;
	}

	if (minus)
		*n = -(*n);

	*n = *n << frac;

	if (*cp != '\0')
	{

		if (*cp != '.')
		{
			/* unexpected character */
			return 1;
		}

		cp++;

		fracLen = 0;
		while ((digit = convertDigit(*cp, radix)) != -1)
		{
			fracDigits[fracLen++] = digit;
			cp++;
		}

		if (*cp != '\0')
		{
			/* unexpected character */
			return 1;
		}

		fracVal = 0;
		for (; fracLen > 0; fracLen--)
		{
			fracVal = (fracVal + (fracDigits[fracLen-1] * (0x1<<(frac+1)))) / radix;
		}

		fracVal += 1;
		fracVal = fracVal >> 1;


		if (minus)
			*n -= fracVal;
		else
			*n += fracVal;
	}

	oldN = *n;
	truncate (n, word);
	if (*n != oldN)
	{
		/* overflow */
		return 2;
	}

	return 0;
}

#if defined (TESTMAIN)

int verbose = 0;

char	*testData[] =
{
	"2 10.01101 7 5",	"overflow",
	"2 010.01101 8 5",	"4d",

	"10 1.5 8 6",		"60",
	"10 2 8 6",		"overflow",
	"10 -1.5 8 6",		"a0",
	"10 -2 8 6",		"80",
	"10 1.5 8 6",		"60",
	"10 1.75 8 6",		"70",
	"10 1.875 8 6",		"78",
	"10 1.9375 8 6",	"7c",
	"10 1.96875 8 6",	"7e",
	"10 1.984375 8 6",	"7f",
	"10 1.9921874 8 6",	"7f",
	"10 1.9921875 8 6",	"overflow",
	"10 1.5 6 4",		"18",
	"10 1.49 6 4",		"18",
	"10 1.51 6 4",		"18",
	"10 1.53124 6 4",	"18",
	"10 1.53125 6 4",	"19"
};

int	testLen = sizeof(testData)/sizeof(*testData)/2;

void	printHex
(
	char	*s,
	int	n,
	int	word
)
{
	char	t[100];
	int	a;
	int	i;

	strcpy (s, "");
	for (i=(word+3)/4; i>0; i--)
	{
		a = (n>>(4*(i-1))) & LSB_1(4);
		sprintf (t, "%x", a);
		strcat (s, t);
	}
}

int	main ()
{
	char	str[100];
	char	res[100];
	int	n;
	int	radix;
	int	word;
	int	frac;
	int	i;
	int	failures=0;
	int	status;

	for (i=0; i<testLen; i++)
	{
		sscanf (testData[i*2], "%d %s %d %d", &radix, str, &word, &frac);

		if ((radix < 2) || (word <= 0) || (frac > word) || (frac <= 0))
			fprintf (stdout, "bad test\n");

		status = convert (&n, word, frac, str, radix);

		switch (status) {
		case 1:		strcpy (res, "bad string");
				break;
		case 2: 	strcpy (res, "overflow");
				break;
		default:	printHex (res, n, word);
		}

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
