#include "argparser.h"
#include <iostream>
#include <string>
#include <cstdlib>

int parseThreadCount(int argc, char* argv[]) {
	int threadCount = getAvailableCores();
	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if ((arg == "-t" || arg == "--threads") && i + 1 < argc) {
			try {
				int userThreads = std::stoi(argv[i + 1]);
				if (userThreads > 0) {
					threadCount = userThreads;
				}
			} catch (const std::invalid_argument&) {
				std::cerr << "Invalid thread count: " << argv[i + 1] << ". Using default value.\n";
			} catch (const std::out_of_range&) {
				std::cerr << "Thread count out of range: " << argv[i + 1] << ". Using default value.\n";
			}
			++i;
		}
	}
	return threadCount;
}

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

bool parseJSONFileArg(int argc, char* argv[], SearchOptions& options) {
	for (int i = 1; i < argc; i++) {
			std::string arg = argv[i];
			if ((arg == "-j" || arg == "--json") && i + 1 < argc) {
					std::string jsonFilePath = argv[i + 1];
					if (jsonFilePath.empty()) {
							std::cerr << "Error: JSON file path is empty.\n";
							return false;
					}

					// Check if the file has a .json extension
					if (jsonFilePath.substr(jsonFilePath.find_last_of('.') + 1) != "json") {
							std::cerr << "Error: File is not a valid JSON file.\n";
							return false;
					}

					return parseJSONInput(jsonFilePath, options);
			}
	}
	return false;
}
