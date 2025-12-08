#pragma once
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

struct LevelData {
	Vector2f SpawnPoint;
	vector <Vector2f> WallPoints;
	vector <Vector2f>EnemySpawnPoints;
	vector <Vector2f> TrapSpawnPoints;
	Vector2f KeySpawnPoint;
	Vector2f DoorSpawnPoint;
};