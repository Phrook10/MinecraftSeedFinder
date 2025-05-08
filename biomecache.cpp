#include "BiomeCache.h"

void BiomeCache::storeNode(int x, int z, int biomeID) {
	const int MAX_COORDINATE = 30000000;  // Minecraft world coordinate limit (±30 million)
	
	// Check that the coordinates are within the valid Minecraft world limits
	if (x < -MAX_COORDINATE || x > MAX_COORDINATE || z < -MAX_COORDINATE || z > MAX_COORDINATE) {
			std::cerr << "Error: Coordinates out of valid range (±30 million). Skipping entry.\n";
			return;  // Skip storing invalid points
	}
	
	// Otherwise, store the valid node
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