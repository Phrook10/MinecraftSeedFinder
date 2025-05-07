#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <thread>
#include "cubiomes/biomes.h"

const std::unordered_map<std::string, BiomeID>& getBiomeNameToID();																			// biome name to ID mapping
MCVersion parseMCVersion(const std::string& versionStr);                                                // returns one of the named constants from biome.h MCVersion enum
int getAvailableCores();                                                                                // returns the number of processor cores as detected by harddwaree_concurrency, or fails back to 1 core

#endif