#pragma once
#include "LevelData.hpp"
#include "BoxML.h"
#include <vector>

class LevelManager {
    LevelData levelData;
    BoxML& boxWorld;

    bool _levelLoaded = false;

    std::vector<bfObject*> spawnedObjects;
    bfPlayer* spawnedPlayer = nullptr;

public:
    LevelManager(const LevelData& lvl, BoxML& bw)
        : levelData(lvl),
        boxWorld(bw)
    {}

    ~LevelManager() {
        unloadLevel();
    }

    void loadLevel() {
        if (_levelLoaded) return;

        // Player
        {
            b2Vec2 boxCoors = boxWorld.pixelToMeter(levelData.player.spawnPos);
            b2Vec2 p = boxCoors;
            spawnedPlayer = boxWorld.CreatePlayer(b2_dynamicBody, p, { 1.f, 1.f });
            if (spawnedPlayer)
                spawnedObjects.push_back(spawnedPlayer);
                cout << "Player spawned at: (" << p.x << ", " << p.y << ")\n";
        }

        // Walls
        for (auto& w : levelData.walls) {
            b2Vec2 boxCoors = boxWorld.pixelToMeter(w.spawnPos);
            b2Vec2 pos = boxCoors;
            bfWall* wall = boxWorld.CreateWall(b2_staticBody, pos, w.size);
            if (wall)
                spawnedObjects.push_back(wall);
        }

        // Enemies
        for (auto& e : levelData.enemies) {
            b2Vec2 boxCoors = boxWorld.pixelToMeter(e.spawnPos);
            b2Vec2 pos = boxCoors;
            bfMonster* mon = boxWorld.CreateMonster(b2_dynamicBody, pos, { e.radius, e.radius });
            if (mon)
                spawnedObjects.push_back(mon);
        }

        _levelLoaded = true;
    }

    void unloadLevel() {
        if (!_levelLoaded) return;

        /*for (auto* obj : spawnedObjects)
            delete obj;*/

        spawnedObjects.clear();
        spawnedPlayer = nullptr;
        _levelLoaded = false;
    }

    bool isLevelLoaded() const {
        return _levelLoaded;
    }
};