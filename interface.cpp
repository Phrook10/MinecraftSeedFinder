#include <iostream>
#include <algorithm>
#include <iomanip>
#include <windows.h>
#include "interface.h"
#include "seedfinder.h"
#include "config.h"
#include "cubiomes/finders.h"
#include "cubiomes/generator.h"

SearchOptions getInputFromUser() {

	SearchOptions searchOptions;

	std::cout << "Enter starting seed (default = 0): ";
	std::cin >> searchOptions.startSeed;
	std::cout << "Enter seed range (default = 10 000): ";
	std::cin >> searchOptions.seedsToCount;

	const auto& biomeMap = getBiomeNameToID(); 

	std::string userInput;
	while (true) {
		std::cout << "\nEnter a biome ID (or type 'done' to finish): ";
		std::cin >> userInput;

		std::transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);

		if (userInput == "done") {
			break;
		}

		auto it = biomeMap.find(userInput); 

		if (it != biomeMap.end()) {
			int maxDist = 0;
			std::cout << "Enter max distance for this biome: ";
			std::cin >> maxDist;

			searchOptions.constraints.push_back({it->second, maxDist});
			std::cout << "Biome constraint added: " << userInput
				<< " with max distance " << maxDist << " from origin\n";
		} else {
			std::cout << "Invalid biome name. Please try again.\n";
		}

		std::cout << "Entered: " << userInput << "\n";
	}

	return searchOptions;
}

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


void setCursorVisibility(bool visible) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void hideCursor() {
	setCursorVisibility(false);
}

void showCursor() {
	setCursorVisibility(true);
}