#include "seedfinder.h"
#include "interface.h"
#include "biomecache.h"
#include "config.h"
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>

// Helper function to get biome name from ID
// This leverages the existing getBiomeNameToID function from config.cpp
std::string getBiomeName(int biomeID) {
    // Get the name-to-ID map from the existing function
    const auto& nameToID = getBiomeNameToID();
    
    // Find the name for this ID by iterating through the map
    for (const auto& [name, id] : nameToID) {
        if (id == biomeID) {
            return name;
        }
    }
    
    // If not found, return a default string with the ID
    return "unknown_biome_" + std::to_string(biomeID);
}

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
bool seedMeetsConstraints(Generator& g, const SearchOptions& options, uint64_t seed, std::unordered_map<int, std::pair<int, int>>& foundBiomes) {
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
    foundBiomes.clear();

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

    // All constraints satisfied
    return true;
}

// loops through a range of seeds starting from startSeed, checks each seed against the constraints
uint64_t findMatchingSeed(const SearchOptions& options, MCVersion version) {
    uint64_t startSeed = options.startSeed;
    uint64_t seedsToCount = options.seedsToCount;
    
    Generator g;
    setupGenerator(&g, version, 0);
    
    hideCursor();
    
    std::vector<SeedMatchResult> matchingSeeds;
    
    for (uint64_t seed = startSeed; seed < startSeed + seedsToCount; ++seed) {
        uint64_t seedsChecked = (seed + 1) - startSeed;
        printProgressBar(seedsChecked, seedsToCount);
        
        std::unordered_map<int, std::pair<int, int>> foundBiomes;
        if (seedMeetsConstraints(g, options, seed, foundBiomes)) {
            // Store the result instead of printing immediately
            SeedMatchResult result;
            result.seed = seed;
            result.biomeLocations = foundBiomes;
            
            // Store biome names for each found biome ID
            for (const auto& [biomeID, _] : foundBiomes) {
                result.biomeNames[biomeID] = getBiomeName(biomeID);
            }
            
            matchingSeeds.push_back(result);
        }
    }
    
    showCursor();
    
    // Print results at the end
    if (!matchingSeeds.empty()) {
        printResults(matchingSeeds);
        writeResultsToLog(matchingSeeds, options);
        return matchingSeeds[0].seed; // Return first matching seed
    } else {
        printf("\nNo matching seed was found in the range of %" PRId64 " to %" PRId64 ".\n", 
               (int64_t)startSeed, (int64_t)(startSeed + seedsToCount));
        return UINT64_MAX;
    }
}

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

//multithread specific functions
void seedSearchWorker(uint64_t startSeed, uint64_t endSeed, const SearchOptions& options,
                     MCVersion version, ThreadSearchData* sharedData) {
    // Create a generator instance for this thread
    Generator g;
    setupGenerator(&g, version, 0);

    // Temporary storage for matching seeds found by this thread
    std::vector<SeedMatchResult> threadMatches;

    // Iterate through seed range assigned to this thread
    for (uint64_t seed = startSeed; seed < endSeed; seed++) {
        // Check if another thread found a seed already (if we only need one)
        if (sharedData->seedFound) {
            break;
        }

        std::unordered_map<int, std::pair<int, int>> foundBiomes;
        // Check if this seed meets all constraints
        if (seedMeetsConstraints(g, options, seed, foundBiomes)) {
            // Create result record
            SeedMatchResult result;
            result.seed = seed;
            result.biomeLocations = foundBiomes;
            
            // Store biome names for each found biome ID
            for (const auto& [biomeID, _] : foundBiomes) {
                result.biomeNames[biomeID] = getBiomeName(biomeID);
            }
            
            // Add to local thread results
            threadMatches.push_back(result);
            
            // Set the first found seed for compatibility
            if (!sharedData->seedFound) {
                sharedData->foundSeed = seed;
                sharedData->seedFound = true;
            }
        }

        // Increment the counter of seeds checked
        sharedData->seedsChecked++;

        // Update progress every 1000 seeds
        if (seed % 1000 == 0) {
            std::lock_guard<std::mutex> lock(sharedData->progressMutex);
            printProgressBar(sharedData->seedsChecked, options.seedsToCount);
        }
    }
    
    // Add all matches to shared results
    if (!threadMatches.empty()) {
        std::lock_guard<std::mutex> lock(sharedData->resultsMutex);
        for (const auto& result : threadMatches) {
            sharedData->matchingSeeds.push_back(result);
        }
    }
}

uint64_t findMatchingSeedThreaded(const SearchOptions& options, MCVersion version, int threadCount) {
    ThreadSearchData sharedData;
    std::vector<std::thread> threads;

    // Calculate seeds per thread
    uint64_t seedRange = options.seedsToCount;
    uint64_t seedsPerThread = seedRange / threadCount;
    hideCursor();

    // Start threads with appropriate seed ranges
    for (int i = 0; i < threadCount; i++) {
        uint64_t threadStartSeed = options.startSeed + (i * seedsPerThread);
        uint64_t threadEndSeed;

        // Make sure the last thread covers any remaining seeds
        if (i == threadCount - 1) {
            threadEndSeed = options.startSeed + seedRange;
        } else {
            threadEndSeed = threadStartSeed + seedsPerThread;
        }

        // Launch thread with its seed range
        threads.push_back(std::thread(seedSearchWorker, threadStartSeed, threadEndSeed, 
                std::ref(options), version, &sharedData));
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    // Make sure progress bar is complete
    if (sharedData.foundSeed != UINT64_MAX) {
        std::lock_guard<std::mutex> lock(sharedData.progressMutex);
        printProgressBar(options.seedsToCount, options.seedsToCount);
    } else {
        std::lock_guard<std::mutex> lock(sharedData.progressMutex);
        printProgressBar(sharedData.seedsChecked, options.seedsToCount);
    }

    showCursor();
    
    // Print all matching seeds found by all threads
    if (!sharedData.matchingSeeds.empty()) {
        printResults(sharedData.matchingSeeds);
        writeResultsToLog(sharedData.matchingSeeds, options);
        return sharedData.foundSeed; // Return first found seed for compatibility
    } else {
        printf("\nNo matching seed was found in the range of %" PRId64 " to %" PRId64 ".\n",
               (int64_t)options.startSeed, (int64_t)(options.startSeed + options.seedsToCount));
        return UINT64_MAX;
    }
}

// Print all matching seed results
void printResults(const std::vector<SeedMatchResult>& results) {
    printf("\n\n===== MATCHING SEEDS FOUND =====\n");
    printf("Found %zu seeds that match all constraints:\n\n", results.size());
    
    for (const auto& result : results) {
        printf("Seed %" PRId64 " matches all constraints:\n", (int64_t)result.seed);
        
        for (const auto& [biomeID, coords] : result.biomeLocations) {
            auto it = result.biomeNames.find(biomeID);
            const std::string& biomeName = (it != result.biomeNames.end()) ? it->second : "unknown_biome";
            
            printf("  Found biome %d (%s) at (%d, %d)\n", 
                   biomeID, 
                   biomeName.c_str(),
                   coords.first, coords.second);
        }
        printf("\n");
    }
}

// Write results to log file
void writeResultsToLog(const std::vector<SeedMatchResult>& results, const SearchOptions& options) {
    std::ofstream logFile(options.logFilePath, std::ios::app);
    
    if (!logFile.is_open()) {
        printf("Warning: Could not open log file '%s' for writing.\n", options.logFilePath.c_str());
        return;
    }
    
    // Get current time for the log
    time_t now = time(nullptr);
    char timeBuffer[80];
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    logFile << "===== SEED SEARCH RESULTS - " << timeBuffer << " =====\n";
    logFile << "Search range: " << options.startSeed << " to " << (options.startSeed + options.seedsToCount - 1) << "\n";
    logFile << "Found " << results.size() << " seeds that match all constraints:\n\n";
    
    for (const auto& result : results) {
        logFile << "Seed " << result.seed << " matches all constraints:\n";
        
        for (const auto& [biomeID, coords] : result.biomeLocations) {
            auto it = result.biomeNames.find(biomeID);
            const std::string& biomeName = (it != result.biomeNames.end()) ? it->second : "unknown_biome";
            
            logFile << "  Found biome " << biomeID << " (" << biomeName << ") at (" 
                    << coords.first << ", " << coords.second << ")\n";
        }
        logFile << "\n";
    }
    
    logFile << "================================\n\n";
    logFile.close();
}
