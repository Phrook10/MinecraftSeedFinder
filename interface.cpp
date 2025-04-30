#include <iostream>
#include <algorithm>
#include <iomanip>
#include <windows.h>
#include "interface.h"
#include "seedfinder.h"
#include "config.h"
#include "cubiomes/finders.h"
#include "cubiomes/generator.h"


// ----- private functions -----

// attempts to validate the seed that the user entered at the prompt
static bool validateUserSeed(const std::string& input, int64_t& result) {
	std::istringstream iss(input);
	iss >> result;

	// fail is stream didn't get through the whole string, or there is any error
	return !iss.fail() && iss.eof();
}

// gets the seed from the user as a string, validates and then returns it as a uint64_t
static uint64_t getSeedFromUser(uint64_t defaultSeed) {
	std::string userInput;
	int64_t signedSeed;

	while (true) {
		std::getline(std::cin, userInput);

		if (userInput.empty()) {
			return defaultSeed;
		} 
		else if (validateUserSeed(userInput, signedSeed)) {
			break;																																											// good input
		} else {
			std::cout << "Invalid input. please enter a valid integer\n";
		}
	}
	return static_cast<uint64_t>(signedSeed);
}

// gets the range of seeds to search from the user as a string, validates it and returns it as a uint64_t
static uint64_t getSeedRangeFromUser(uint64_t defaultSeedRange) {

	std::string userInput;
	int64_t signedSeedRange;

	while (true) {
		std::getline(std::cin, userInput);

		if (userInput.empty()) {
			return defaultSeedRange;
		}
		else if (validateUserSeed(userInput, signedSeedRange)) {
			break;																																											// good input
		} else {
			std::cout << "Invalid input. please enter a valid integer\n";
		}
	}
	return static_cast<uint64_t>(signedSeedRange);
}

// gets the maximum distance from origin a biome should be as a string, validates it, and then returns the distance as an int
static int getBiomeMaxDistanceFromUser(int defaultBiomeMaxDistance) {
	std::string userInput;
	int64_t signedInput;

	while (true) {
		std::getline(std::cin, userInput);

		if (userInput.empty()) {
			return defaultBiomeMaxDistance;
		}
		else if (validateUserSeed(userInput, signedInput)) {
			// We don't want to accept a negative distance for this variable
			if (signedInput < 0 || signedInput > std::numeric_limits<int>::max()) {
				std::cout << "Please enter a positive number.\n";
			} else {
				break;
			}
		} else {
			std::cout << "Invalid input. Please enter a valid integer.\n";
		}
	}

	return static_cast<int>(signedInput);
}

// provides the mechanism that hideCursor and showCursor use.
static void setCursorVisibility(bool visible) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(out, &cursorInfo);
}


// ----- public functions -----

// Gets input from the user
SearchOptions getInputFromUser() {

	SearchOptions searchOptions;

	// get starting seed (default = 0)
	std::cout << "Enter starting seed (default = 0): ";
	searchOptions.startSeed = getSeedFromUser(searchOptions.startSeed);
	std::cout << "Enter seed range (default = 1000): ";
	searchOptions.seedsToCount = getSeedRangeFromUser(searchOptions.seedsToCount);

	const auto& biomeMap = getBiomeNameToID(); 

	std::string userInput;
	while (true) {
		std::cout << "\nEnter a biome ID (or type 'done' to finish): ";
		std::getline(std::cin, userInput);

		std::transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);

		if (userInput == "done") {
			break;
		}

		auto it = biomeMap.find(userInput); 

		if (it != biomeMap.end()) {
			const int defaultMaxDist = BiomeConstraint{}.maxDistance;
			std::cout << "Enter max distance for this biome (default = " << defaultMaxDist << "): ";
			int maxDist = getBiomeMaxDistanceFromUser(defaultMaxDist);

			searchOptions.constraints.push_back({it->second, maxDist});
			std::cout << "Biome constraint added: " << userInput << " with max distance " << maxDist << " from origin\n";					// consider revising this message 										
		} else {
			std::cout << "Invalid biome name. Please try again.\n";
		}

		std::cout << "Entered: " << userInput << "\n";																																					// this line is useless
	}

	return searchOptions;
}

// progress bar related functions
// prints the progress to the screen to provide visual feedback to the user
void printProgressBar(long long current, long long total) {
	const int barWidth = 50;
	float progress = static_cast<float>(current) / total;
	int pos = static_cast<int>(barWidth * progress);
  
	std::cout << "\r[";
	for (int i = 0; i < barWidth; ++i) {
		if (i < pos) std::cout << "#";
		else if (i == pos) std::cout << ">";
		else std::cout << " ";
	}
	std::cout << "] "
	          << std::setw(6) << current << " / " << total
	          << " (" << std::setw(3) << static_cast<int>(progress * 100) << "%)"
	          << std::flush;
}

// Cursor hiding functions

void hideCursor() {
	setCursorVisibility(false);
}

void showCursor() {
	setCursorVisibility(true);
}