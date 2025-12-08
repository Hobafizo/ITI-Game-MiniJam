#pragma once
#include "LevelData.hpp"
class LevelManager {
	LevelData levelData; // we can make it a list of levels
	public:
		LevelManager(LevelData l1) {
		levelData = l1;
		};
	~LevelManager();

	void loadLevel(int levelNumber) {
		levelData.EnemySpawnPoints;//spawn at this location
		levelData.SpawnPoint;//spawn at this location
		levelData.TrapSpawnPoints;//spawn at this location with orientation
		levelData.WallPoints;//spawn walls at these locations with orientation
		levelData.DoorSpawnPoint;//spawn at this location
		levelData.KeySpawnPoint;//spawn at this location
	};
	void unloadLevel(int levelNumber);
	bool isLevelLoaded(int levelNumber) const;
};