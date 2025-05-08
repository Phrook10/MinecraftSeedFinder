#include "argparser.h"
#include <iostream>
#include <string>
#include <cstdlib>

// Function to parse command line arguments for thread count
int parseThreadCount(int argc, char* argv[]) {
	int threadCount = getAvailableCores();
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

// Function to parse command line arguments for Minecraft version
MCVersion parseVersionArg(int argc, char* argv[]) {
	MCVersion version = MC_NEWEST;
	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if ((arg == "-v" || arg == "--version") && i + 1 < argc) {
			version = parseMCVersion(argv[i + 1]);
			++i;
		}
	}
	return version;
}

// Function to parse command line arguments for JSON input file
bool parseJSONFileArg(int argc, char* argv[], SearchOptions& options) {
	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if ((arg == "-j" || arg == "--json") && i + 1 < argc) {
			std::string jsonFilePath = argv[i + 1];
			return parseJSONInput(jsonFilePath, options);
		}
	}
	return false;
}
