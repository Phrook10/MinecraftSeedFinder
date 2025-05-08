#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include "seedfinder.h"
#include "cubiomes/biomes.h"

void printHelp();                                                                                       // prints the help menu in the command prompt 
const std::unordered_map<std::string, BiomeID>& getBiomeNameToID();																			// biome name to ID mapping
MCVersion parseMCVersion(const std::string& versionStr);                                                // returns one of the named constants from biome.h MCVersion enum
int getAvailableCores();                                                                                // returns the number of processor cores as detected by harddwaree_concurrency, or fails back to 1 core
bool parseJSONInput(const std::string& filepath, SearchOptions& options);                                // parses a json file the user enters at the command line for rapid searching

#endif