#ifndef BIOMECACHE_H
#define BIOMECAChE_H


#include <vector>
#include <utility>
#include <iostream>

// individual coordinate node
struct BiomeResult {
  std::pair<int, int> position;
  int biomeID;
};

class BiomeCache {
  public:
	void storeNode(int x, int z, int biomeID);                    // stores a node in the cache
	int getBiomeID(int x, int z) const;                           // retreives the biome id at a specified coordinate
	bool doesCoordExist(int x, int z) const;                      // checks if a coordinate exists in the cache
	void clearCache();                                            // clears the cache
	const std::vector<BiomeResult>& getAllPoints() const;         // dumps all data in the cache

private:
	std::vector<BiomeResult> results;                             // contains the results of the intial search
};

#endif