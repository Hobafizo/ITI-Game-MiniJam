#pragma once

class LevelManager {
	public:
	LevelManager();
	~LevelManager();

	void loadLevel(int levelNumber);
	void unloadLevel(int levelNumber);
	bool isLevelLoaded(int levelNumber) const;
};