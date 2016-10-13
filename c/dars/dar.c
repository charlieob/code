#include <stdio.h>

static int protectedData;

void	setProtectedData (v)
int	v;
{
	protectedData = v;
}

int	getProtectedData ()
{
	return(protectedData);
}
