#pragma once
#include "LevelData.hpp"
#include "ObjectCategory.h"

static LevelData Level1Data = {

    // Player
    { sf::Vector2f(500.f, 100.f) },

    // Walls (converted to Level3 format)
    {
        { sf::Vector2f(960.f, 200.f),  sf::Vector2f(WALL_VERTICAL_WIDTH, WALL_VERTICAL_HEIGHT),  ObjectCategory::Wall_Vertical },
        { sf::Vector2f(500.f, 540.f),  sf::Vector2f(WALL_VERTICAL_WIDTH, WALL_VERTICAL_HEIGHT), ObjectCategory::Wall_Vertical },
    },

    // Enemies (converted, still empty)
    {
        // none
    },

    // Traps
    {},

    // Key (added size)
    { sf::Vector2f(1400.f, 650.f), { 71, 82 } },

    // Door (added size)
    { sf::Vector2f(1400.f, 100.f), { 202, 298 } }
};