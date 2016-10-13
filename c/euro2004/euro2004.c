#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*******

Guardian Unlimited: Oct 16th, 2003

http://football.guardian.co.uk/news/theknowledge/0,9204,1064263,00.html

"Can't help thinking that the whole Euro play-off 2004 draw is a bit dodgy,"
says conspiracy theorist Adrian Higgins. "It all looks very convenient for
the bigger nations. I mean just what are the chances of Turkey, Holland,
Spain, Russia and Croatia all avoiding each other?"

A whopping 250-1, according to Ben Morgan - who points out that it's not the
first time this sort of thing has happened.

"The last Euro play-off draw was again supposed to be unseeded and yet Denmark
(then very high in the world) drew Israel, England drew Scotland, Ukraine (then
also high in the world) drew Slovenia (and lost!) and rising stars Turkey drew
falling stars the Republic of Ireland," he says.

"Having heard stories of 'warm balls' being used in the past to signify the
seeds in the pot, I only wish a TV company had taken a heat vision camera to
the draw," concludes Ben, possibly with a sly chuckle.



Consider a draw of 10 teams, with 5 'seeded' teams. What are the chances in a
straight draw of these 10 teams that no seeded teams will play each other?

There are 945 possible outcomes.

draw with 2 teams: 1 possible outcome
draw with 4 teams: 3 possible outcomes (ab cd, ac bd, ad bc)
draw with 6 teams: 5 possible outcomes

In general:
O(n) = n-1 * O(n-2)
O(2) = 1

Therefore for 10 teams, it's 9*7*5*3*1 = 945

How many of the possible outcomes have no seeded teams playing each other?

Consider seeded teams A1-A5 and unseeded ones B1-B5.

A1  5 ways
A2  4
A3  3
A4  2
A5  1

i.e. there are 5*4*3*2*1 such outcomes = 120.

Therefore the probability is 120/945 = 0.12698 (12.7%).
This is odds of 6.875 to 1.


Note that one of the 5 'stronger' teams is Croatia (are they really stronger than
all of Norway, Slovenia, Wales, Scotland, Latvia?).  If there are only 4 'seeded'
teams, then the number of outcomes in which none of these play each other is
6*5*4*3 = 360

Now the probability is 360/945 = 0.38095 (38.1%).  (See alternative code below.)
This is odds of 1.625 to 1.


********/

#define MATCHES 5
#define TEAMS (MATCHES*2)
#define TRIES 100000000

main ()
{
	int a[TEAMS];
	int i,j,k;
	char clash;
	unsigned int easy=0;
	time_t t1;
	long seed;

	//printf ("RAND_MAX=%0x\n", RAND_MAX);

	time (&t1);
	seed = (long) t1;
	srandom (seed);

	for (i=0; i<TRIES; i++)
	{
		for (j=0; j<TEAMS; j++)
			a[j] = 0;

		for (j=1; j<=TEAMS; j++)
		{
			do {
				k = random() % TEAMS;
			} while (a[k] != 0);

			a[k] = j;
		}

/***

Assume teams 1,2,3,4,5 are seeded

***/
		clash = 0;
		for (j=0; j<MATCHES; j++)
		{
			if (a[2*j] <= TEAMS/2 && a[2*j+1] <= TEAMS/2)
//			if (a[2*j] <= TEAMS/2-1 && a[2*j+1] <= TEAMS/2-1)	// only 4 seeds
				clash = 1;
		}
/*
		for (j=0; j<TEAMS; j++)
			printf ("%d ", a[j]);
		if (clash)
			printf ("!\n");
		else
			printf ("\n");
*/
		if (!clash)
			easy++;

		if (((i+1) % (TRIES/10)) == 0)
			printf ("after %d tries: %f\n", i+1, (float) easy/(i+1));
	}

	printf ("total = %u, easy = %u, hard = %u\n", TRIES, easy, TRIES-easy);
	printf ("probability = %f\n", (float) easy/TRIES);
}
