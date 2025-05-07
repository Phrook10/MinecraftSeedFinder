#ifndef SEEDFINDER_H
#define SEEDFINDER_H
#include <vector>
#include <cstdint>
#include <string>
#include <utility>
#include <cmath>
#include <atomic>
#include <mutex>
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
  uint64_t seedsToCount = 1000;
  bool randomizeStartSeed = false;
  std::vector<BiomeConstraint> constraints;
};

// Struct for thread data sharing
struct ThreadSearchData {
  std::atomic<bool> seedFound{false};
  std::atomic<uint64_t> foundSeed{UINT64_MAX};
  std::mutex progressMutex;
  std::atomic<uint64_t> seedsChecked{0};
};

// functions
bool seedMeetsConstraints(const Generator& g, const SearchOptions& options, uint64_t seed);           // given a specific seed, generator, and options, return true if the seed meets all the biome constraints
uint64_t findMatchingSeed(const SearchOptions& options, MCVersion version);                           // loops through a range of seeds starting from startSeed, checks each seed againts the constraints, and returns the first one that matches
uint64_t findMatchingSeedThreaded(const SearchOptions& options, MCVersion version, int threadCount);  // multithreaded version of findMatchingSeed


// Thread worker function
void seedSearchWorker(uint64_t startSeed, uint64_t endSeed, const SearchOptions& options, 
                      MCVersion version, ThreadSearchData* sharedData);

// algorithm specific functions
std::vector<std::pair<int, int>> generateSpiralCoords(int maxDist, int step = 64);                      // returns a vector of coordinate pairs that form a spiral pattern around the origin

// getters
const SearchOptions& getSearchOptions();

#endif