#include <stdio.h>

/*
--
-- Some fairly simple code for handling command-line arguments.
-- Options handled are a, b and h (b has a mandatory argument).
-- Following the optional arguments, it is mandatory that there
-- should be at least one (file) argument.
--
-- The following command lines are all correctly handled:
--	args -a		// an error as no file argument
--	args -a b cc
--	args -a -b cc dd
--	args -a -bcc dd
--	args -ab cc dd
--	args -abc dd
--	...
--
*/

int main (int argc, char *argv[])
{
	char c, option_a=0, option_b=0;
	char* option_b_arg;
	char* progname = *argv;

	while (--argc > 0 && (*++argv)[0] == '-')

		while (c = *++argv[0])

			switch (c) {

			case 'a':
				option_a = 1;
				break;

			case 'b':
				option_b = 1;

				if (*++argv[0])
					option_b_arg = *argv;
				else
				{
					--argc;
					option_b_arg = *++argv;
				}

				while (*++argv[0]);
				--argv[0];

				break;

			case 'h':
				argc = 0;
				break;

			default:
				printf ("%s: illegal option %c\n", progname, c);
				argc = 0;
				break;
			}

	if (argc < 1)
		printf ("Usage: %s [-ah] [-b arg_b] file[s]\n", progname);
	else
	{
		if (option_a)
			printf ("option a, ");
		if (option_b)
		{
			printf ("option b (arg=%s), ", option_b_arg);
		}

		printf ("args: ");
		while (argc-- > 0)
			printf ("%s ", *argv++);
		printf ("\n");
	}

	return 0;
}
