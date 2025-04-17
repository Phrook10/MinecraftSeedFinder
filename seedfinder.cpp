#include "seedfinder.h"


// given a specific seed, generator, and options, return true if the seed meets all the biome constraints
bool seedMeetsConstraints(Generator& g, const SearchOptions& options, uint64_t seed) {
	applySeed(&g, DIM_OVERWORLD, seed);

	for (const BiomeConstraint& constraint : options.constraints) {
		int targetBiome = constraint.BiomeID;
		int maxDist = constraint.maxDistance;
		int yCoord = 63;
		int coordinateIncrement = 64;
		int scale = 1;
		bool found = false;

		for (int x = -maxDist; x <= maxDist; x += coordinateIncrement) {
			for (int z = -maxDist; z <= maxDist; z += coordinateIncrement) {
				int currentBiomeID = getBiomeAt(&g, scale, x, yCoord, z);
				if (currentBiomeID == targetBiome) {
          printf("Found biome %d at (%d, %d) for seed %" PRId64 "\n", targetBiome, x, z, (int64_t)seed);
					found = true;
					break;
				}
			}
			if (found) break;
		}

		if (!found) {
			return false;                                                                       // if any constraint fails
		}
	}

	return true;                                                                            // all constraints passed
}

// loops through a range of seeds starting from startSeed, checks each seed againts the constraints, and returns the first one that matches
uint64_t findMatchingSeed(const SearchOptions& options) {

  uint64_t startSeed = options.startSeed;
  uint64_t seedsToCount = options.seedsToCount;

  for (uint64_t seed = startSeed; seed < startSeed + seedsToCount; ++seed) {
    Generator g;
    if (seedMeetsConstraints(g, options, seed)) {
      printf("Seed %" PRId64 " matches all constraints. \n", (int64_t)seed);
      return seed; 
    }
  }
  // if no matching seed is found in range
  printf("no matching seed was found in the range of %" PRId64 " to %" PRId64 ".\n", (int64_t)startSeed, (int64_t)(startSeed + seedsToCount));
  return 0;
};
