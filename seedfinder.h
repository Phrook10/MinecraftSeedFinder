#ifndef SEEDFINDER_H
#define SEEDFINDER_H
#include <vector>
#include <cstdint>
#include <string>
#include <utility>
#include <cmath>
#include <atomic>
#include <mutex>
#include <fstream>
#include <unordered_map>
#include "cubiomes/finders.h"
#include "cubiomes/generator.h"

// variables
// a single search constraint
struct BiomeConstraint {
    int BiomeID;
    int maxDistance = 500;
};

// Structure to store seed match results
struct SeedMatchResult {
    uint64_t seed;
    std::unordered_map<int, std::pair<int, int>> biomeLocations;
    std::unordered_map<int, std::string> biomeNames; 
};

// groups all constraints and options together
struct SearchOptions {
    uint64_t startSeed = 0;
    uint64_t seedsToCount = 1000;
    bool randomizeStartSeed = false;
    std::vector<BiomeConstraint> constraints;
    std::string logFilePath = "seed_results.log";
};

// Struct for thread data sharing
struct ThreadSearchData {
    std::atomic<bool> seedFound{false};
    std::atomic<uint64_t> foundSeed{UINT64_MAX};
    std::mutex progressMutex;
    std::atomic<uint64_t> seedsChecked{0};
    std::mutex resultsMutex;
    std::vector<SeedMatchResult> matchingSeeds;
};

// functions
bool seedMeetsConstraints(Generator& g, const SearchOptions& options, uint64_t seed, std::unordered_map<int, std::pair<int, int>>& foundBiomes);
uint64_t findMatchingSeed(const SearchOptions& options, MCVersion version);
uint64_t findMatchingSeedThreaded(const SearchOptions& options, MCVersion version, int threadCount);

// Thread worker function
void seedSearchWorker(uint64_t startSeed, uint64_t endSeed, const SearchOptions& options,
                     MCVersion version, ThreadSearchData* sharedData);

// algorithm specific functions
std::vector<std::pair<int, int>> generateSpiralCoords(int maxDist, int step = 64);

// Log and output functions
void writeResultsToLog(const std::vector<SeedMatchResult>& results, const SearchOptions& options);
void printResults(const std::vector<SeedMatchResult>& results);

// getters
const SearchOptions& getSearchOptions();

#endif