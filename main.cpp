/* 
* This program is designed to brute force analyze Minecraft world seeds.*
* Initially it will work for Java Edition, and eventually to some extent*
* Bedrock edition. A GUI is planned as well.                            *
*/

// main.cpp
#include <stdio.h>
#include <stdlib.h>
#include "cubiomes/finders.h"
#include "cubiomes/generator.h"

int main() {
	Generator g;
	setupGenerator(&g, MC_1_21_3, 0);

  uint64_t seed;                                                            // seeds are internally represented as unsigned 64bit integers
	for (seed = 0; seed < 1000000; seed++) {
		
    // Push the seed to the generator for the overworld
    applySeed(&g, DIM_OVERWORLD, seed);

    int scale = 1;                                                          // scale = 1: block coordinate, scale = 4: biome coordinates
    int x = 0, y = -20, z = 0;                                               // spawn might not be at world origin, but it will most likely be close. Consider refining logic later
    int biomeID = getBiomeAt(&g, scale, x, y, z);
    if(biomeID == lush_caves)
    {
      printf("Seed %" PRId64 " has a Mushroom Fields biome at block position (%d, %d).\n", (int64_t) seed, x, z);
      break;
    }

  
  }
	return 0;
}
