#pragma once
#include "LevelData.hpp"
#include "BoxML.h"
#include <vector>
//1920.f-1366.f
//1080.f-768.f
float shiftFactorX=0;
float shiftFactorY=0;
class LevelManager {
    LevelData levelData;
    BoxML& boxWorld;

    bool _levelLoaded = false;

    std::vector<bfObject*> spawnedObjects;
    bfPlayer* spawnedPlayer = nullptr;

public:
    LevelManager( BoxML& bw)
        :
        boxWorld(bw)
    {}

    ~LevelManager() {
        unloadLevel();
    }

    void loadLevel(const LevelData& lvl) {
        levelData = lvl;
        cout << "new level loaded";
        if (_levelLoaded) return;

        // Player remember to uncomment, having an issue currently
        {
            /*Vector2f shiftedPos = Vector2f(levelData.player.spawnPos.x+shiftFactorX, levelData.player.spawnPos.y + shiftFactorY);
            b2Vec2 boxCoors = boxWorld.pixelToMeter(shiftedPos);
            b2Vec2 p = boxCoors;
            spawnedPlayer = boxWorld.CreatePlayer(b2_dynamicBody, p, 1.f);
            if (spawnedPlayer)
                spawnedObjects.push_back(spawnedPlayer);
                cout << "Player spawned at: (" << p.x << ", " << p.y << ")\n";*/
        }

        // Walls
        for (auto& w : levelData.walls) {
            Vector2f shiftedPos = Vector2f(w.spawnPos.x + shiftFactorX, w.spawnPos.y + shiftFactorY);

            b2Vec2 boxCoors = boxWorld.pixelToMeter(shiftedPos);
            b2Vec2 pos = boxCoors;
            bfWall* wall = boxWorld.CreateWall(b2_staticBody, pos, w.size);
            if (wall)
                spawnedObjects.push_back(wall);
        }

        // Enemies
        for (auto& e : levelData.enemies) {
            Vector2f shiftedPos = Vector2f(e.spawnPos.x + shiftFactorX, e.spawnPos.y + shiftFactorY);

            b2Vec2 boxCoors = boxWorld.pixelToMeter(shiftedPos);
            b2Vec2 pos = boxCoors;
            bfMonster* mon = boxWorld.CreateMonster(b2_dynamicBody, pos, e.radius);
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