#pragma once
#include "LevelData.hpp"
#include "BoxML.h"
#include <vector>
#include <iostream>
#include "bfKey.h"
#include "bfDoor.h"
//1920.f-1366.f
//1080.f-768.f
float shiftFactorX= (1920.f - 1366.f) / 2.f;;
float shiftFactorY= (1080.f - 768.f) / 2.f;
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
        
        if (_levelLoaded) return;
        cout << "new level loaded";
        // Player remember to uncomment, having an issue currently
        float playWidth = 1366.f;
        float playHeight = 768.f;

        float centerX = playWidth * 0.5f;
        float centerY = playHeight * 0.5f;

        float shiftX = shiftFactorX;
        float shiftY = shiftFactorY;

        // Decide shift direction based on player position
        if (levelData.player.spawnPos.x > centerX)
            shiftX = -abs(shiftX);
        else
            shiftX = abs(shiftX);

        if (levelData.player.spawnPos.y > centerY)
            shiftY = -abs(shiftY);
        else
            shiftY = abs(shiftY);

        // Lambda to shift any position
        auto shifted = [&](Vector2f p) {
            return Vector2f(p.x + shiftX, p.y + shiftY);
            };

        // ---------------- Player ----------------
        bfPlayer* spawnedPlayer;
        {
            Vector2f sp = shifted(levelData.player.spawnPos);
            b2Vec2 p = boxWorld.pixelToMeter(sp);

                spawnedPlayer = boxWorld.CreatePlayer(
                b2_dynamicBody,
                p,
                { 165, 192 },
                0.01f,
                0.3f
            );

            if (spawnedPlayer) {
                spawnedPlayer->Body()->SetLinearVelocity({ PLAYER_SPEED_X, PLAYER_SPEED_Y });
                spawnedObjects.push_back(spawnedPlayer);
            }
        }

        // ---------------- Walls ----------------
        bfWall* wall;
        for (auto& w : levelData.walls) {
            Vector2f sp = shifted(w.spawnPos);
            b2Vec2 pos = boxWorld.pixelToMeter(sp);

                wall = boxWorld.CreateWall(
                b2_staticBody,
                pos,
                w.size,
                0.01f,
                0.3f,
                (uint16)w.type
            );

            if (wall)
                spawnedObjects.push_back(wall);
        }

        // ---------------- Enemies ----------------
        bfMonster* mon;
        for (auto& e : levelData.enemies) {
            Vector2f sp = shifted(e.spawnPos);
            b2Vec2 pos = boxWorld.pixelToMeter(sp);

            mon = boxWorld.CreateMonster(
                b2_dynamicBody,
                pos,
                e.size,
                0.01f,
                0.3f,
                (unsigned char)e.type
            );

            if (mon) {
                spawnedObjects.push_back(mon);
            }
        }

        // ---------------- Key ----------------
        bfKey* key;
        {
            Vector2f sp = shifted(levelData.key.spawnPos);
            b2Vec2 pos = boxWorld.pixelToMeter(sp);

                key = boxWorld.CreateKey(
                b2_staticBody,
                pos,
                levelData.key.size,
                0.01f,
                0.3f
            );

            if (key)
                spawnedObjects.push_back(key);
        }

        // ---------------- Door ----------------
        bfDoor* door;
        {
            Vector2f sp = shifted(levelData.door.spawnPos);
            b2Vec2 pos = boxWorld.pixelToMeter(sp);

            door = boxWorld.CreateDoor(
                b2_staticBody,
                pos,
                levelData.door.size,
                0.01f,
                0.3f
            );

            if (door)
                spawnedObjects.push_back(door);
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