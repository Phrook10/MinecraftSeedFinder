/*
* This program is designed to brute force analyze Minecraft world seeds.*
* Initially it will work for Java Edition, and eventually to some extent*
* Bedrock edition. A GUI is planned as well.                            *
*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <string>
#include "seedfinder.h"
#include "config.h"
#include "interface.h"
#include "multithreading.h"
#include "cubiomes/finders.h"
#include "cubiomes/generator.h"

// Function to parse command line arguments for thread count
int parseThreadCount(int argc, char* argv[]) {
    // Default to system-detected core count
    int threadCount = getAvailableCores();
    
    // Check for command line override
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if ((arg == "-t" || arg == "--threads") && i + 1 < argc) {
            int userThreads = std::stoi(argv[i + 1]);
            if (userThreads > 0) {
                threadCount = userThreads;
            }
            ++i;
        }
    }
    
    return threadCount;
}

int main(int argc, char* argv[]) {
    MCVersion version = MC_NEWEST;
    
    // Detect available cores
    int availableCores = getAvailableCores();
    int threadCount = parseThreadCount(argc, argv);
    
    // Simple argument parser
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-v" || arg == "--version") && i + 1 < argc) {
            version = parseMCVersion(argv[i + 1]);
            ++i;
        }
        else if ((arg == "-t" || arg == "--threads") && i + 1 < argc) {
            // Already handled in parseThreadCount
            ++i;
        }
    }
    
    // Display core information
    std::cout << "Detected " << availableCores << " CPU cores." << std::endl;
    
    if (threadCount != availableCores) {
        std::cout << "Using " << threadCount << " threads for seed searching." << std::endl;
    } else {
        std::cout << "Using all available cores for seed searching." << std::endl;
    }
 
    std::cout << "Minecraft Seed Finder\n";
    SearchOptions userInput = getInputFromUser();
    
    std::cout << "Searching Seeds...\n";
    
    // Use the threaded finder instead of the single-threaded version
    uint64_t result = findMatchingSeedThreaded(userInput, version, threadCount);
    
    if (result != UINT64_MAX) {
        std::cout << "\nMatching seed found: " << result << "\n";
    } else {
        std::cout << "\nNo matching seed found in the given range.\n";
    }
		std::cout << "Press Enter to Exit...\n";
		std::cin.get();
    return 0;
}