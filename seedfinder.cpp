#include "seedfinder.h"
#include "interface.h"
#include "biomecache.h"
#include <unordered_map>
#include <unordered_set>

// finds the largest maxDistance from all constraints
int getLargestMaxDistance(const std::vector<BiomeConstraint>& constraints) {
	int largest = 0;
	for (const BiomeConstraint& constraint : constraints) {
		if (constraint.maxDistance > largest) {
			largest = constraint.maxDistance;
		}
	}
	return largest;
}

// given a specific seed, generator, and options, return true if the seed meets all the biome constraints
bool seedMeetsConstraints(Generator& g, const SearchOptions& options, uint64_t seed) {
	applySeed(&g, DIM_OVERWORLD, seed);

	const int yCoord = 63;
	const int coordinateIncrement = 16;
	const int scale = 1;

	const std::vector<BiomeConstraint>& constraints = options.constraints;
	const int maxDist = getLargestMaxDistance(constraints);

	BiomeCache biomeCache;
	std::vector<std::pair<int, int>> spiralCoords = generateSpiralCoords(maxDist, coordinateIncrement);

	// Step 1: Build the biome cache
	for (const auto& [x, z] : spiralCoords) {
		int biomeID = getBiomeAt(&g, scale, x, yCoord, z);
		biomeCache.storeNode(x, z, biomeID);
	}

	// Step 2: Check each constraint and store found positions
	std::unordered_map<int, std::pair<int, int>> foundBiomes;

	for (const BiomeConstraint& constraint : constraints) {
		bool found = false;
		int maxDist = constraint.maxDistance;

		for (const auto& [x, z] : spiralCoords) {
			if (std::abs(x) > maxDist || std::abs(z) > maxDist) continue;

			int cachedBiomeID = biomeCache.getBiomeID(x, z);
			if (cachedBiomeID == constraint.BiomeID) {
				foundBiomes[constraint.BiomeID] = {x, z};
				found = true;
				break;
			}
		}

		if (!found) {
			return false; // One constraint not satisfied, bail out
		}
	}

	// Step 3: All constraints found, print results
	printf("Seed %" PRId64 " matches all constraints.\n", (int64_t)seed);
	for (const auto& [biomeID, coord] : foundBiomes) {
		printf("  Found biome %d at (%d, %d)\n", biomeID, coord.first, coord.second);
	}

	return true;
}


// loops through a range of seeds starting from startSeed, checks each seed againts the constraints, and returns the first one that matches
uint64_t findMatchingSeed(const SearchOptions& options, MCVersion version) {

  uint64_t startSeed = options.startSeed;
  uint64_t seedsToCount = options.seedsToCount;
	version = version;

	Generator g;
	setupGenerator(&g, version, 0);

	hideCursor();																																
  for (uint64_t seed = startSeed; seed < startSeed + seedsToCount; ++seed) {

		uint64_t seedsChecked = (seed + 1) - startSeed;
		printProgressBar(seedsChecked, seedsToCount);
    if (seedMeetsConstraints(g, options, seed)) {
			showCursor();
      printf("Seed %" PRId64 " matches all constraints. \n", (int64_t)seed);
      return seed; 
    }
  }
  // if no matching seed is found in range
	showCursor();
  printf("\nNo matching seed was found in the range of %" PRId64 " to %" PRId64 ".\n", (int64_t)startSeed, (int64_t)(startSeed + seedsToCount));
  return UINT64_MAX;
};


//algorithm specific functions
//spiral search pattern generator
std::vector<std::pair<int, int>> generateSpiralCoords(int maxDist, int step) {
	std::vector<std::pair<int, int>> coords;
	int x = 0, z = 0;
	int dx = 0, dz = -step;
	int segmentLength = 1;

	while (std::abs(x) <= maxDist && std::abs(z) <= maxDist) {
		for (int i = 0; i < 2; ++i) { 
			for (int j = 0; j < segmentLength; ++j) {
				if (std::abs(x) <= maxDist && std::abs(z) <= maxDist) {
					coords.emplace_back(x, z);
				}
				x += dx;
				z += dz;
			}
			// rotate direction 90 degrees
			int tmp = dx;
			dx = -dz;
			dz = tmp;
		}
		segmentLength += step;
	}

	return coords;
}