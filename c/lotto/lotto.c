#include <stdio.h>

#define	N	39
#define	S	6
#define	I	10000

int	randomN (void);
void	makeWinningSelection (unsigned char*, unsigned char*);
void	makeSelection (unsigned char*);
void	writeSelection (FILE*, unsigned char*, unsigned char);
int	matchSelections (unsigned char*, unsigned char*);
int	matchBonus (unsigned char*, unsigned char);

int	randomN (void)
{
	return random()%N;
}

void	makeWinningSelection
(
	unsigned char	*sel,
	unsigned char	*bonus
)
{
	int i,n;

	for (i=0; i<N; i++)
		sel[i] = 0;

	n=0;
	while (n < S)
	{
		i = randomN();
		if (!sel[i])
		{
			sel[i] = 1;
			n++;
		}
	}

	n=0;
	while (n == 0)
	{
		i = randomN();
		if (!sel[i])
		{
			*bonus = i;
			n++;
		}
	}
}

void	makeSelection
(
	unsigned char	*sel
)
{
	int i,n;

	for (i=0; i<N; i++)
		sel[i] = 0;

	n=0;
	while (n < S)
	{
		i = randomN();
		if (!sel[i])
		{
			sel[i] = 1;
			n++;
		}
	}
}

void	writeSelection
(
	FILE		*fp,
	unsigned char	*sel,
	unsigned char	 bonus
)
{
	int i;

	for (i=0; i<N; i++)
	{
		if (sel[i])
			fprintf (fp, "%d ", i+1);
	}
	fprintf (fp, "(%d)\n", bonus+1);
}

int	matchSelections
(
	unsigned char	*sel1,
	unsigned char	*sel2
)
{
	int i,n;

	n=0;
	for (i=0; i<N; i++)
	{
		if (sel1[i])
			if (sel2[i])
				n++;
	}

	return n;
}

int	matchBonus
(
	unsigned char	*sel,
	unsigned char	 bonus
)
{
	return sel[bonus];
}

main
(
	int	argc,
	char	*argv[]
)
{
	unsigned char selection[N],s[N],bonus;
	int seed,tries;
	int i,i1,i2,i3,i4,i5,i6;
	int m,mb,m0,m1,m2,m3;

	if (argc == 1)
	{
		seed = 1;
		tries = I;
	}
	else if (argc == 2)
	{
		seed = atoi (argv[1]);
		tries = I;
	}
	else if (argc == 3)
	{
		seed = atoi (argv[1]);
		tries = atoi (argv[2]);
	}
	else
	{
		fprintf (stderr, "too many arguments\n");
		exit (1);
	}

	srandom(seed);
	makeWinningSelection (selection, &bonus);
	writeSelection (stdout, selection, bonus);

	m0=m1=m2=m3=0;

#if defined(EXHAUSTIVE)

	for (i1=0; i1<N; i1++)
		s[i1]=0;

	tries=0;

	for (i1=0; i1<N-S+1; i1++)
	{
	 s[i1]=1;
	 for (i2=i1+1; i2<N-S+2; i2++)
	 {
	  s[i2]=1;
	  for (i3=i2+1; i3<N-S+3; i3++)
	  {
	   s[i3]=1;
	   for (i4=i3+1; i4<N-S+4; i4++)
	   {
	    s[i4]=1;
	    for (i5=i4+1; i5<N-S+5; i5++)
	    {
	     s[i5]=1;
	     for (i6=i5+1; i6<N-S+6; i6++)
	     {
	      s[i6]=1;

	      m = matchSelections (selection, s);
              mb = matchBonus (s, bonus);

              switch (m)
              {
              case S:	m0++; break;
              case S-1:	m1++; break;
              case S-2:	m2++; break;
              case S-3:	if (mb) m3++; break;
              default:	break;
              }

              tries++;

	      s[i6]=0;
	     }
	     s[i5]=0;
	    }
	    s[i4]=0;
	   }
	   s[i3]=0;
	  }
	  s[i2]=0;
	 }
	 s[i1]=0;
	}

#else

	for (i=0; i<tries; i++)
	{
		makeSelection (s);
		m = matchSelections (selection, s);
		mb = matchBonus (s, bonus);

		switch (m)
		{
		case S:		m0++; break;
		case S-1:	m1++; break;
		case S-2:	m2++; break;
		case S-3:	if (mb) m3++; break;
		default:	break;
		}
	}

#endif

	fprintf (stdout, "combinations tried = %d\n", tries);
	fprintf (stdout, "match %2d           = %d\n", S, m0);
	fprintf (stdout, "match %2d           = %d\n", S-1, m1);
	fprintf (stdout, "match %2d           = %d\n", S-2, m2);
	fprintf (stdout, "match %2d and bonus = %d\n", S-3, m3);
}
