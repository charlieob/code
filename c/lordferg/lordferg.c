#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*******

Fergie said:

"It was a nice draw for the Spanish and Italians, I think they picked it themselves. 
"The three Italian teams have avoided each other and so have the three Spanish. How do you think that worked out? 
"I can tell you, Uefa don't want us in the final, that's for sure. 
"I don't know why they have given the final to Old Trafford because they don't want us to get there." 

So what exactly are the chances of a draw of 8 teams, containing 3 Spanish
teams and 3 Italian teams, producing the outcome that made Sir Alex so cross?

My calculations are that there are 105 possible outcomes in total and
that 42 of these have no derbies, i.e. a probability of 40% that Fergie
will be cross.

draw with 4 teams: 3 possible outcomes (ab cd, ac bd, ad bc)
draw with 6 teams: 5 possible outcomes

In general:
O(n) = n-1 * O(n-2)
O(2) = 1

Therefore for 8 teams, it's 7*5*3*1 = 105

Assume abc are all Spanish
   and fgh are all Italian

We will try to find out how many outcomes have no derbies.

How many outcomes have abc derbies?
ab x 15
ac x 15
bc x 15
This is 45 in total.

Similarly 45 outcomes have fgh derbies.

But how many have both?
Look at the abc derbies.

ab: fg x 3
    fh x 3
    gh x 3
i.e. 9 of the 'ab' outcomes have a fgh derby as well.
Also 9 for the 'ac' and 'bc' outcomes.
This is 27 in total.
Therefore 27 of 45 abc derbies also have fgh derbies,
and hence 18 outcomes have abc derbies only.

Similarly 18 outcomes have fgh derbies only.


Total possible outcomes     105
# with both derbies          27
# with abc derbies only      18
# with fgh derbies only      18
-------------------------------
# with no derbies = 105 - 27 - 18 - 18 = 42

The above is simulated in the ifdef STRAIGHTDRAW part of the code.


But, the Champions League draw for the quarter-finals has 2 further
constraints:
(1) each Q/F has one group winner against a runner-up
(2) teams who met in the 2nd group stage are kept apart in the Q/F

Consider the 8 teams:
a1    b1
a2    b2
a3    b3
a4    b4
where a1 is the winner of group1 and a2 the runner-up.

If we ignore constraint (2) for a moment:
The total number of possible draws is 4 x 3 x 2 x 1 = 24.

In the draw, there are 2 Spanish group winners (and 1 r/up) and 1 Italian
group winner (and 2 runners-up). Imagine a1,a2,b1 are Spanish and a4,b3,b4
are Italian.

The number of possible outcomes with Spanish derbies is 2 x 3 x 2 = 12.
(The one Spanish r/up can be drawn against either of the Spanish winners,
with 6 permutations of the rest each.)
Similarly, the number of possible outcomes with Italian derbies is also 12.

How many have both?
Take a1b1 together.
Can now have a4b4 which has 2 x 2 combinations, or a4b3 which has the same.
Therefore there are 2 x 2 x 2 possible outcomes with 2 derbies.

Total possible outcomes     24
# with both derbies          8
# with Sp derbiy only        4
# with It derby only         4
------------------------------
# with no derbies = 24 - 8 - 4 - 4 = 8.

Therfore the probability is 1/3.


Now look at constraint 2 again.  We have to look at the actual line-up
to understand this one.

           Winner        Runner-up
Group 1    Barcelona     Inter
Group 2    Valencia      Ajax
Group 3    AC Milan      Real Madrid
Group 4    MUFC          Juventus

The number of possible outcomes of the draw with constraint (2) is 9:
Consider a1b4.
There are only 3 possible combinations of the other 3 pairs.
Similarly for a1b3 and a1b2.
Therefore total possible outcomes is 3 x 3 = 9.
(I'm sure there's a more elegant derivation of the above but I can't think of it.)

Now, how many of these 9 outcomes have no derbies?
This is so specific that I can't think of any way to do it other than to
write them all out.  If you do this, you find that the only possible outcome
is:
Barcelona - Juventus
Valencia  - Inter
Ac Milan  - Ajax
Man U     - Real Madrid

This, amazingly, is the actual outcome of the draw that emerged (a 1-in-9
chance). Perhaps Sir Alex was right to be sceptical.

I'm not sure but I think the draw was made by computer rather than drawing balls.
This is probably necessary as it would be possible to reach a deadlock in the
draw after 3 ties have been drawn due to constraint (2), and then you would have
to start again. So the question is whether the computer was programmed to avoid
derbies, which is not provided for in the rules.

The #else code below simulates this, and demonstrates that the probability is 1/9.

********/

#define TRIES 100000000

#ifdef STRAIGHTDRAW
main ()
{
	int a[8];
	int i,j,k;
	char match;
	unsigned int cross=0;
	time_t t1;
	long seed;

	time (&t1);
	seed = (long) t1;
	srand (seed);

	for (i=0; i<TRIES; i++)
	{
		for (j=0; j<8; j++)
			a[j] = 0;

		for (j=1; j<=8; j++)
		{
			do {
				k = (rand() >> 11) & 0x07;   // mod 8
			} while (a[k] != 0);

			a[k] = j;
		}

/***

Assume teams 1,2,3 are all Spanish
   and teams 6,7,8 are all Italian

***/
		match = 0;
		for (j=0; j<4; j++)
		{
			if (a[2*j] <= 3 && a[2*j+1] <= 3)
				match = 1;

			if (a[2*j] >= 6 && a[2*j+1] >= 6)
				match = 1;
		}
/*
		for (j=0; j<8; j++)
			printf ("%d ", a[j]);
		if (match)
			printf ("!\n");
		else
			printf ("\n");
*/
		if (!match)
			cross++;

		if (((i+1) % (TRIES/10)) == 0)
			printf ("after %d tries: %f\n", i+1, (float) cross/(i+1));
	}

	printf ("total = %u, happy = %u, cross = %u\n", TRIES, TRIES-cross, cross);
	printf ("probability = %f\n", (float) cross/TRIES);
}

#else


main ()
{
	int a[4];
	int i,j,k;
	char match;
	unsigned int total=0,cross=0,failed=0;
	time_t t1;
	long seed;

	time (&t1);
	seed = (long) t1;
	srand (seed);

	for (i=0; i<TRIES; i++)
	{
		for (j=0; j<4; j++)
			a[j] = 0;

		for (j=1; j<=3; j++)
		{
			do {
				k = (rand() >> 11) & 0x03;   // mod 4
			} while (a[k] != 0 /*|| k+1 == j*/);

			a[k] = j;
		}

		k = 0;
		while (a[k] != 0)
			k++;
		a[k] = 4;

		if (a[0] == 1 ||
		    a[1] == 2 ||
		    a[2] == 3 ||
		    a[3] == 4)
			failed++;
		else
		{
			total++;

			match = (a[0] == 3 ||
				 a[1] == 3 ||
				 a[2] == 1 ||
				 a[2] == 4);
/*
			for (j=0; j<4; j++)
				printf ("%d ", a[j]);

			if (match)
				printf ("!\n");
			else
				printf ("\n");
*/
			if (!match)
				cross++;
		}

		if (((i+1) % (TRIES/10)) == 0)
			printf ("after %d tries: %f\n", i+1, (float) cross/total);
	}

	printf ("total = %u, happy = %u, cross = %u\n", total, total-cross, cross);
	printf ("probability = %.9f\n", (double) cross/total);
	printf ("deviation (from 1/9) = %.4f %%\n", 100 * fabs(((double)9*cross/total) - 1));
}


#endif
