/* 
* This program is designed to brute force analyze Minecraft world seeds.*
* Initially it will work for Java Edition, and eventually to some extent*
* Bedrock edition. A GUI is planned as well.                            *
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "seedfinder.h"
#include "config.h"
#include "cubiomes/finders.h"
#include "cubiomes/generator.h"

int main() {

  SearchOptions searchOptions;

  std::cout << "Program started.\n";

  std::cout << "Minecraft Biome Finder\n\n";
  std::cout << "Enter starting seed (default = 0): ";
  std::cin >> searchOptions.startSeed;
  std::cout << "Enter seed range (default = 10 000): ";
  std::cin >> searchOptions.seedsToCount;

  // capture biome constraints
  std::string userInput;
  while (true) {
    std::cout << "\nEnter a biome ID (or type 'done' to finish): ";
    std::cin >> userInput;

    // Convert the input to lowercase using std::transform
    std::transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);

    if (userInput == "done") {
        break; 
    }

    // look up the biome (unordered_map in config.h)
    auto it = biomeNameToID.find(userInput);

    if (it != biomeNameToID.end()) {
      // biome has been matched to the enum, ask for max distance
      int maxDist = 0;
      std::cout << "Enter max distance for this biome: ";
      std::cin >> maxDist;

      // Add the constraint to the search options
      searchOptions.constraints.push_back({it->second, maxDist});
      std::cout << "Biome constraint added: " << userInput << " with max distance " << maxDist << " from origin\n";
    } else {
      std::cout << "Invalid biome name. Please try again.\n";
    }
  }
  findMatchingSeed(searchOptions);

  std::cout << "\nPress Enter to exit...";
  std::cin.ignore();
  std::cin.get();
	return 0;
}
