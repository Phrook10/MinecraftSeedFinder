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
			std::string jsonFilePath = argv[++i];

			if (jsonFilePath.empty()) {
				std::cerr << "Error: JSON file path is empty.\n";
				exit(EXIT_FAILURE);
			}

			if (jsonFilePath.substr(jsonFilePath.find_last_of('.') + 1) != "json") {
				std::cerr << "Error: File '" << jsonFilePath << "' is not a valid JSON file.\n";
				exit(EXIT_FAILURE);
			}

			if (!parseJSONInput(jsonFilePath, options)) {
				std::cerr << "Error: Failed to parse JSON file '" << jsonFilePath << "'.\n";
				exit(EXIT_FAILURE);
			}

			return true;
		}
	}
	return false;
}

void parseSearchModeArg(int argc, char* argv[], SearchOptions& options) {
	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if ((arg == "-m" || arg == "--mode") && i + 1 < argc) {
			std::string mode = argv[++i];

			if (mode == "SINGLE_MATCH") {
				options.searchMode = SINGLE_MATCH;
			} else if (mode == "ALL_MATCHES") {
				options.searchMode = ALL_MATCHES;
			} else {
				std::cerr << "Error: Invalid search mode '" << mode << "'. Valid modes are SINGLE_MATCH, ALL_MATCHES.\n";
				exit(EXIT_FAILURE);
			}
		}
	}
}
