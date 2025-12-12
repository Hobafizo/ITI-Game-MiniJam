#pragma once
#include "LevelData.hpp"
#include "ObjectCategory.h"
static LevelData Level3Data = {

    // Player position in pixels
    // (500, 100) means the player starts 500px from the left and 100px from the top.
    { sf::Vector2f(960.f, 900.f) },

    // Wall data: list of walls with spawn position and size in pixels.
    // These walls are thin because the sizes are small.
    // Example: size (100, 2) pixels becomes (10m x 0.2m) after conversion.
    {
    // Vertical wall (you already had this one correct)
    { sf::Vector2f(960.f, 700.f),
      sf::Vector2f(WALL_VERTICAL_HEIGHT,WALL_VERTICAL_WIDTH),
      0.f,
      ObjectCategory::Wall_Vertical },

      // Horizontal wall (100 wide, 20 tall)
      { sf::Vector2f(960.f, 200.f),
        sf::Vector2f(WALL_VERTICAL_HEIGHT,WALL_VERTICAL_WIDTH),
        0.f,
        ObjectCategory::Wall_Vertical },

        // Vertical wall (20 wide, 100 tall)
        { sf::Vector2f(910.f, 100.f),
          sf::Vector2f(WALL_VERTICAL_WIDTH,WALL_VERTICAL_HEIGHT),
          0.f,
          ObjectCategory::Wall_Vertical },

          // Vertical wall (20 wide, 100 tall)
          { sf::Vector2f(100.f, 300.f),
            sf::Vector2f(WALL_VERTICAL_WIDTH, WALL_VERTICAL_HEIGHT),
            0.f,
            ObjectCategory::Wall_Vertical },
     },
    // Enemy data: enemy spawn position and radius in pixels.
    // radius 1.5f becomes 0.15 meters in Box2D.
    {
        { sf::Vector2f(1300.f, 700.f), 20.5f, 1},
        // EnemyData object 2
        { sf::Vector2f(20.f, 900.f), 20.5f, 2  },
    },

    // Traps (empty for now)
    {},

    // Key position in pixels
    { sf::Vector2f(1800.f, 900.f) },

    // Door position in pixels
    { sf::Vector2f(960.f, 1.f) }
};