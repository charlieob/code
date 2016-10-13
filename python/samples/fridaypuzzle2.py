# -*- coding: utf-8 -*-
import random

# Initialize counts
black = 0;
white = 0;

for count in range (0, 1000000):

  # choose a color for the initial stone (0 or 1)
  stone = random.choice(['black', 'white'])    

  # Now put this, and a white stone, in the bag
  bag = [ stone, 'white' ]

  # Now choose one from the bag
  chosen = random.choice( bag );

  if( chosen == 'black' ):
    black += 1;
  else:
    white += 1;

print "You pulled out ", black, " black and ", white, "white"
