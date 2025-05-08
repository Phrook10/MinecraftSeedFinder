#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <string>
#include "argparser.h"
#include "seedfinder.h"
#include "config.h"
#include "interface.h"
#include "multithreading.h"
#include "cubiomes/finders.h"
#include "cubiomes/generator.h"
#include "include/nlohmann/json.hpp"
using json = nlohmann::json;

int main(int argc, char* argv[]) {
    // Check for help command
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printHelp();
            return 0;
        }
    }

    // Parse command line arguments
    int threadCount = parseThreadCount(argc, argv);
    MCVersion version = parseVersionArg(argc, argv);
    
    // Detect available cores and display information
    int availableCores = getAvailableCores();
    std::cout << "Detected " << availableCores << " CPU cores." << std::endl;
   
    if (threadCount != availableCores) {
        std::cout << "Using " << threadCount << " threads for seed searching." << std::endl;
    } else {
        std::cout << "Using all available cores for seed searching." << std::endl;
    }
 
    std::cout << "Minecraft Seed Finder\n";
    
    // Process input - either from JSON or user interface
    SearchOptions searchOptions;
    bool usingJsonInput = parseJSONFileArg(argc, argv, searchOptions);
    
    if (!usingJsonInput) {
        // No JSON file or JSON parsing failed, use interactive mode
        searchOptions = getInputFromUser();
    } else {
        // JSON file was successfully parsed, display summary
        std::cout << "Loaded search parameters from JSON file:\n";
        std::cout << "- Starting seed: " << searchOptions.startSeed << "\n";
        std::cout << "- Seeds to check: " << searchOptions.seedsToCount << "\n";
        std::cout << "- Constraints: " << searchOptions.constraints.size() << "\n";
        
        for (size_t i = 0; i < searchOptions.constraints.size(); i++) {
            std::cout << "  " << (i+1) << ". Biome ID: " << searchOptions.constraints[i].BiomeID
                     << ", Max Distance: " << searchOptions.constraints[i].maxDistance << "\n";
        }
    }
   
    std::cout << "Searching Seeds...\n";
   
    // Use the threaded finder instead of the single-threaded version
    uint64_t result = findMatchingSeedThreaded(searchOptions, version, threadCount);
   
    if (result != UINT64_MAX) {
        std::cout << "\nMatching seed found: " << result << "\n";
    } else {
        std::cout << "\nNo matching seed found in the given range.\n";
    }
    
    // Only wait for user input if we're not using JSON mode (interactive mode)
    if (!usingJsonInput) {
        std::cout << "Press Enter to Exit...\n";
        std::cin.get();
    }
    
    return 0;
}