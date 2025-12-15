#pragma once
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "bfPlayer.h"
#include "bfMonster.h"
#include "bfWall.h"
#include "ObjectCategory.h"
#include "def.h"

using namespace std;
using namespace sf;
struct PlayerData {
    Vector2f spawnPos;
};

struct WallData {
    Vector2f spawnPos;
    Vector2f size;
    ObjectCategory type;
};

struct EnemyData {
    Vector2f spawnPos;
    Vector2f size;
    unsigned char type;
};

struct KeyData {
    Vector2f spawnPos;
    Vector2f size ={ 71, 82 };

};

struct DoorData {
    Vector2f spawnPos;
    Vector2f size = { 202, 298 };
};

struct LevelData {
    PlayerData player;
    vector<WallData> walls;
    vector<EnemyData> enemies;
    vector<Vector2f> traps;
    KeyData key;
    DoorData door;
};