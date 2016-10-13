typedef	unsigned int	mpchunk;

typedef struct
{
	int		len;
	mpchunk		*val;
} mpnum;

#define	MPCHUNKSIZE	(CHAR_BIT * sizeof (mpchunk))
