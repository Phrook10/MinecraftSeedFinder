#ifndef SEEDFINDER_H
#define SEEDFINDER_H

#include <vector>
#include <cstdint>
#include <string>
#include "cubiomes/finders.h"
#include "cubiomes/generator.h"

// variables

// a single search constraint
struct BiomeConstraint {
  int BiomeID;
  int maxDistance = 500;
};

// groups all constraints and options together
struct SearchOptions {
  uint64_t startSeed = 0;
  uint64_t seedsToCount = 10000;
  bool randomizeStartSeed = false;
  std::vector<BiomeConstraint> constraints;
};

// functions

bool seedMeetsConstraints(const Generator& g, const SearchOptions& options, uint64_t seed);     // given a specific seed, generator, and options, return true if the seed meets all the biome constraints
uint64_t findMatchingSeed(const SearchOptions& options);                                        // loops through a range of seeds starting from startSeed, checks each seed againts the constraints, and returns the first one that matches

// getters
const SearchOptions& getSearchOptions();



#endif