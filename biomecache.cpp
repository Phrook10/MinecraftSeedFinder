#include "BiomeCache.h"

void BiomeCache::storeNode(int x, int z, int biomeID) {
	results.push_back({{x, z}, biomeID});
}

int BiomeCache::getBiomeID(int x, int z) const {
	for (const auto& result : results) {
		if (result.position.first == x && result.position.second == z)
			return result.biomeID;
	}
	return -1; // Not found or use optional/error code
}

bool BiomeCache::doesCoordExist(int x, int z) const {
	for (const auto& result : results) {
		if (result.position.first == x && result.position.second == z)
			return true;
	}
	return false;
}

void BiomeCache::clearCache() {
	results.clear();
}

const std::vector<BiomeResult>& BiomeCache::getAllPoints() const {
	return results;
}