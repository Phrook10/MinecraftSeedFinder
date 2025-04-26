#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>
#include <string>
#include "cubiomes/biomes.h"

const std::unordered_map<std::string, BiomeID>& getBiomeNameToID();																			// biome name to ID mapping

#endif