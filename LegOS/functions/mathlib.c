#include "../include/mathlib.h"

#include <stdint.h>
#include <stddef.h>


//no math.h no square roots :( , gotta make it yourself:
int sqrt(int square) {
	double guess = square / 2;
	for (size_t i = 0; i < 10; i++) //Heron's method for the win
	{
		guess = 0.5 * (guess + (square / guess));
	}
	int result = guess;
	return result;
} 



/*
	when using this function to graph a circle, changing the number of calculations to something like 1000-3000 makes it an animation lol, and an entierly unnecessary one at that,
	concidering 10 loops results in the same accuracy at a fraction of a fraction of the time. But "it's not a bug it's a feature" lol
	so I can effectivly control the animation speed & length if I include the number of loops as a paremiter and create some 'starting up'
	animations, while having done 0 implementations to actual video functionalities.
*/


