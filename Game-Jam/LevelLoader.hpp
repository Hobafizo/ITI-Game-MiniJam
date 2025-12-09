#include "bfPlayer.h"
#include "bfMonster.h"
#include "bfWall.h"
#include "LevelData.hpp"
class LevelLoader {
public:
    static bfPlayer* createPlayer(const PlayerData& data, b2World& world) {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(data.spawnPos.x, data.spawnPos.y);

        b2Body* body = world.CreateBody(&bodyDef);

        float radius = 0.5f;   // or from LevelData
        return new bfPlayer(body, radius, 0xFFFFFF);
    }

    static bfWall* createWall(const WallData& data, b2World& world) {
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set(data.spawnPos.x, data.spawnPos.y);
        bodyDef.angle = data.orientation;

        b2Body* body = world.CreateBody(&bodyDef);

        return new bfWall(body, data.size, 0x888888);
    }

    static bfMonster* createMonster(const EnemyData& data, b2World& world) {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(data.spawnPos.x, data.spawnPos.y);

        b2Body* body = world.CreateBody(&bodyDef);

        return new bfMonster(body, data.radius, data.color);
    }
};