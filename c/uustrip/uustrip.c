/*
-- removes mail headers from a file containing a multi-part
-- file transfer.
-- Assumptions:
--	(1) each message header begins with a line starting
--	    with "From"
--	(2) each header terminates with a blank line
*/

#include <stdio.h>

#define	N	80

int	getline
(
	char	*line,
	int	max
)
{
	if (fgets (line, max, stdin) == NULL)
		return 0;
	else
		return strlen (line);
}

main ()
{
	short finished=0;
	short mailheader=0;
	int len;
	char line[N];

	while (!finished)
	{
		len = getline (line, N);

		if (len == 0)
			finished = 1;
		else if (len == 1)
			mailheader = 0;
		else if (!mailheader)
			if (!strncmp (line, "From", 4))
				mailheader = 1;
			else
				fputs (line, stdout);
	}
}

