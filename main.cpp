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
#include "interface.h"
#include "cubiomes/finders.h"
#include "cubiomes/generator.h"


int main() {

  std::cout << "In the program\n";
  std::cout << "Minecraft Seed Finder\n";


  SearchOptions userInput = getInputFromUser();
	std::cout << "Searching Seeds";
	uint64_t result = findMatchingSeed(userInput);

	if (result != UINT64_MAX) {
		std::cout << "Matching seed found: " << result << "\n";
	} else {
		std::cout << "No matching seed found in the given range.\n";
	}
	return 0;
 
}
