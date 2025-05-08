#include "config.h"
#include "seedfinder.h"
#include "include/nlohmann/json.hpp"
using json = nlohmann::json;


void printHelp() {
  std::cout << "Minecraft Seed Finder - Help\n";
  std::cout << "-------------------------------\n";
  std::cout << "Command line options:\n";
  std::cout << "  -t, --threads N      Set number of threads to use (default: all available cores)\n";
  std::cout << "  -v, --version VER    Set Minecraft version (default: newest)\n";
  std::cout << "  -j, --json FILE      Load search parameters from JSON file\n";
  std::cout << "  -h, --help           Display this help message\n";
  std::cout << "\n";
  std::cout << "JSON file format example:\n";
  std::cout << "{\n";
  std::cout << "  \"startSeed\": 0,\n";
  std::cout << "  \"seedsToCount\": 1000,\n";
  std::cout << "  \"randomizeStartSeed\": false,\n";
  std::cout << "  \"constraints\": [\n";
  std::cout << "    {\n";
  std::cout << "      \"biomeID\": \"jagged_peaks\",\n";
  std::cout << "      \"maxDistance\": 500\n";
  std::cout << "    },\n";
  std::cout << "    {\n";
  std::cout << "      \"biomeID\": 137,\n";
  std::cout << "      \"maxDistance\": 300\n";
  std::cout << "    }\n";
  std::cout << "  ]\n";
  std::cout << "}\n";
}


// biome name to ID mapping
const std::unordered_map<std::string, BiomeID>& getBiomeNameToID() {
	static const std::unordered_map<std::string, BiomeID> map = {
    {"none", none},
    {"ocean", ocean},
    {"plains", plains},
    {"desert", desert},
    {"mountains", mountains},
    {"extreme_hills", extremeHills},
    {"forest", forest},
    {"taiga", taiga},
    {"swamp", swamp},
    {"swampland", swampland},
    {"river", river},
    {"nether_wastes", nether_wastes},
    {"hell", hell},
    {"the_end", the_end},
    {"sky", sky},
    {"frozen_ocean", frozen_ocean},
    {"frozenOcean", frozenOcean},
    {"frozen_river", frozen_river},
    {"frozenRiver", frozenRiver},
    {"snowy_tundra", snowy_tundra},
    {"icePlains", icePlains},
    {"snowy_mountains", snowy_mountains},
    {"iceMountains", iceMountains},
    {"mushroom_fields", mushroom_fields},
    {"mushroomIsland", mushroomIsland},
    {"mushroom_field_shore", mushroom_field_shore},
    {"mushroomIslandShore", mushroomIslandShore},
    {"beach", beach},
    {"desert_hills", desert_hills},
    {"desertHills", desertHills},
    {"wooded_hills", wooded_hills},
    {"forestHills", forestHills},
    {"taiga_hills", taiga_hills},
    {"taigaHills", taigaHills},
    {"mountain_edge", mountain_edge},
    {"extremeHillsEdge", extremeHillsEdge},
    {"jungle", jungle},
    {"jungle_hills", jungle_hills},
    {"jungleHills", jungleHills},
    {"jungle_edge", jungle_edge},
    {"jungleEdge", jungleEdge},
    {"deep_ocean", deep_ocean},
    {"deepOcean", deepOcean},
    {"stone_shore", stone_shore},
    {"stoneBeach", stoneBeach},
    {"snowy_beach", snowy_beach},
    {"coldBeach", coldBeach},
    {"birch_forest", birch_forest},
    {"birchForest", birchForest},
    {"birch_forest_hills", birch_forest_hills},
    {"birchForestHills", birchForestHills},
    {"dark_forest", dark_forest},
    {"roofedForest", roofedForest},
    {"snowy_taiga", snowy_taiga},
    {"coldTaiga", coldTaiga},
    {"snowy_taiga_hills", snowy_taiga_hills},
    {"coldTaigaHills", coldTaigaHills},
    {"giant_tree_taiga", giant_tree_taiga},
    {"megaTaiga", megaTaiga},
    {"giant_tree_taiga_hills", giant_tree_taiga_hills},
    {"megaTaigaHills", megaTaigaHills},
    {"wooded_mountains", wooded_mountains},
    {"extremeHillsPlus", extremeHillsPlus},
    {"savanna", savanna},
    {"savanna_plateau", savanna_plateau},
    {"savannaPlateau", savannaPlateau},
    {"badlands", badlands},
    {"mesa", mesa},
    {"wooded_badlands_plateau", wooded_badlands_plateau},
    {"mesaPlateau_F", mesaPlateau_F},
    {"badlands_plateau", badlands_plateau},
    {"mesaPlateau", mesaPlateau},
    {"small_end_islands", small_end_islands},
    {"end_midlands", end_midlands},
    {"end_highlands", end_highlands},
    {"end_barrens", end_barrens},
    {"warm_ocean", warm_ocean},
    {"warmOcean", warmOcean},
    {"lukewarm_ocean", lukewarm_ocean},
    {"lukewarmOcean", lukewarmOcean},
    {"cold_ocean", cold_ocean},
    {"coldOcean", coldOcean},
    {"deep_warm_ocean", deep_warm_ocean},
    {"warmDeepOcean", warmDeepOcean},
    {"deep_lukewarm_ocean", deep_lukewarm_ocean},
    {"lukewarmDeepOcean", lukewarmDeepOcean},
    {"deep_cold_ocean", deep_cold_ocean},
    {"coldDeepOcean", coldDeepOcean},
    {"deep_frozen_ocean", deep_frozen_ocean},
    {"frozenDeepOcean", frozenDeepOcean},
    {"seasonal_forest", seasonal_forest},
    {"rainforest", rainforest},
    {"shrubland", shrubland},
    {"the_void", the_void},
    {"sunflower_plains", sunflower_plains},
    {"desert_lakes", desert_lakes},
    {"gravelly_mountains", gravelly_mountains},
    {"flower_forest", flower_forest},
    {"taiga_mountains", taiga_mountains},
    {"swamp_hills", swamp_hills},
    {"ice_spikes", ice_spikes},
    {"modified_jungle", modified_jungle},
    {"modified_jungle_edge", modified_jungle_edge},
    {"tall_birch_forest", tall_birch_forest},
    {"tall_birch_hills", tall_birch_hills},
    {"dark_forest_hills", dark_forest_hills},
    {"snowy_taiga_mountains", snowy_taiga_mountains},
    {"giant_spruce_taiga", giant_spruce_taiga},
    {"giant_spruce_taiga_hills", giant_spruce_taiga_hills},
    {"modified_gravelly_mountains", modified_gravelly_mountains},
    {"shattered_savanna", shattered_savanna},
    {"shattered_savanna_plateau", shattered_savanna_plateau},
    {"eroded_badlands", eroded_badlands},
    {"modified_wooded_badlands_plateau", modified_wooded_badlands_plateau},
    {"modified_badlands_plateau", modified_badlands_plateau},
    {"bamboo_jungle", bamboo_jungle},
    {"bamboo_jungle_hills", bamboo_jungle_hills},
    {"soul_sand_valley", soul_sand_valley},
    {"crimson_forest", crimson_forest},
    {"warped_forest", warped_forest},
    {"basalt_deltas", basalt_deltas},
    {"dripstone_caves", dripstone_caves},
    {"lush_caves", lush_caves},
    {"meadow", meadow},
    {"grove", grove},
    {"snowy_slopes", snowy_slopes},
    {"jagged_peaks", jagged_peaks},
    {"frozen_peaks", frozen_peaks},
    {"stony_peaks", stony_peaks},
    {"old_growth_birch_forest", old_growth_birch_forest},
    {"old_growth_pine_taiga", old_growth_pine_taiga},
    {"old_growth_spruce_taiga", old_growth_spruce_taiga},
    {"snowy_plains", snowy_plains},
    {"sparse_jungle", sparse_jungle},
    {"stony_shore", stony_shore},
    {"windswept_hills", windswept_hills},
    {"windswept_forest", windswept_forest},
    {"windswept_gravelly_hills", windswept_gravelly_hills},
    {"windswept_savanna", windswept_savanna},
    {"wooded_badlands", wooded_badlands},
    {"deep_dark", deep_dark},
    {"mangrove_swamp", mangrove_swamp},
    {"cherry_grove", cherry_grove},
    {"pale_garden", pale_garden}
  };
  return map;
}

MCVersion parseMCVersion(const std::string& versionStr) {
	static const std::unordered_map<std::string, MCVersion> versionMap = {
		{"MC_1_0_0", MC_1_0_0}, {"MC_1_1_0", MC_1_1_0}, {"MC_1_2_5", MC_1_2_5},
		{"MC_1_3_2", MC_1_3_2}, {"MC_1_4_7", MC_1_4_7}, {"MC_1_5_2", MC_1_5_2},
		{"MC_1_6_4", MC_1_6_4}, {"MC_1_7_10", MC_1_7_10}, {"MC_1_8_9", MC_1_8_9},
		{"MC_1_9_4", MC_1_9_4}, {"MC_1_10_2", MC_1_10_2}, {"MC_1_11_2", MC_1_11_2},
		{"MC_1_12_2", MC_1_12_2}, {"MC_1_13_2", MC_1_13_2}, {"MC_1_14_4", MC_1_14_4},
		{"MC_1_15_2", MC_1_15_2}, {"MC_1_16_1", MC_1_16_1}, {"MC_1_16_5", MC_1_16_5},
		{"MC_1_17_1", MC_1_17_1}, {"MC_1_18_2", MC_1_18_2}, {"MC_1_19_2", MC_1_19_2},
		{"MC_1_19_4", MC_1_19_4}, {"MC_1_20_6", MC_1_20_6}, {"MC_1_21_1", MC_1_21_1},
		{"MC_1_21_3", MC_1_21_3}, {"MC_1_21_WD", MC_1_21_WD}, {"MC_1_21", MC_1_21},
		{"MC_NEWEST", MC_NEWEST}
	};

	auto it = versionMap.find(versionStr);
	if (it != versionMap.end()) {
		return it->second;
	}

	std::cerr << "Unknown Minecraft version: " << versionStr << std::endl;
	return MC_NEWEST;
}

int getAvailableCores() {

  int numCores = std::thread::hardware_concurrency();

  // if hardware_concurrency() returns 0 (failed to detect), default to 1 core
  if (numCores == 0) {
    numCores = 1;
  }
  return numCores;
}


// Function to parse JSON input file into SearchOptions struct
bool parseJSONInput(const std::string& filepath, SearchOptions& options) {
  
  const auto& biomeMap = getBiomeNameToID();
  try {
      // Read JSON file
      std::ifstream file(filepath);
      if (!file.is_open()) {
          std::cerr << "Error: Unable to open JSON file: " << filepath << std::endl;
          return false;
      }
      
      json jsonData;
      file >> jsonData;
      
      // Parse startSeed
      if (jsonData.contains("startSeed")) {
          options.startSeed = jsonData["startSeed"].get<uint64_t>();
      }
      
      // Parse seedsToCount
      if (jsonData.contains("seedsToCount")) {
          options.seedsToCount = jsonData["seedsToCount"].get<uint64_t>();
      }
      
      // Parse randomizeStartSeed
      if (jsonData.contains("randomizeStartSeed")) {
          options.randomizeStartSeed = jsonData["randomizeStartSeed"].get<bool>();
      }
      
      // Parse constraints
      if (jsonData.contains("constraints") && jsonData["constraints"].is_array()) {
          options.constraints.clear();
          
          for (const auto& constraint : jsonData["constraints"]) {
              BiomeConstraint biomeConstraint;
              
              // Check for biomeID (required)
              if (!constraint.contains("biomeID")) {
                  std::cerr << "Error: Missing biomeID in constraint" << std::endl;
                  return false;
              }
              
              // Handle biomeID as either string or integer
              if (constraint["biomeID"].is_string()) {
                std::string biomeName = constraint["biomeID"].get<std::string>();
                auto it = biomeMap.find(biomeName);

                if (it != biomeMap.end()) {
                  biomeConstraint.BiomeID = it->second;
                } else {
                  std::cerr << "Error: Invalid biome name: " << biomeName << std::endl;
                  return false;
                }
              } else if (constraint["biomeID"].is_number()) {
                biomeConstraint.BiomeID = constraint["biomeID"].get<int>();
              } else {
                std::cerr << "Error: biomeID must be a string or number" << std::endl;
                return false;
              }
              
              // Parse maxDistance (optional)
              if (constraint.contains("maxDistance")) {
                  biomeConstraint.maxDistance = constraint["maxDistance"].get<int>();
              }
              
              options.constraints.push_back(biomeConstraint);
          }
      }
      
      // Validation: ensure we have at least one constraint
      if (options.constraints.empty()) {
          std::cerr << "Error: JSON file must contain at least one constraint" << std::endl;
          return false;
      }
      
      return true;
  } catch (const json::exception& e) {
      std::cerr << "JSON parsing error: " << e.what() << std::endl;
      return false;
  } catch (const std::exception& e) {
      std::cerr << "Error parsing JSON file: " << e.what() << std::endl;
      return false;
  }
}