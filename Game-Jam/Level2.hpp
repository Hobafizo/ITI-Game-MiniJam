#pragma once
#include "LevelData.hpp"
#include "ObjectCategory.h"

static LevelData Level2Data = {

    // Player
    { sf::Vector2f(960.f, 540.f) },

    // Walls (converted to Level3 format)
    {
        { sf::Vector2f(190.f, 100.f),  sf::Vector2f(WALL_VERTICAL_WIDTH, WALL_VERTICAL_HEIGHT),  ObjectCategory::Wall_Vertical },
        { sf::Vector2f(50.f, 700.f),   sf::Vector2f(WALL_VERTICAL_WIDTH, WALL_VERTICAL_HEIGHT),  ObjectCategory::Wall_Horizontal },
    },

    // Enemies (converted: give size + type)
    {
        { sf::Vector2f(1600.f, 700.f), {50.f, 50.f}, 1 }
    },

    // Traps
    {},

    // Key (added missing size)
    { sf::Vector2f(1800.f, 900.f), {71, 82} },

    // Door (added missing size)
    { sf::Vector2f(95.f, 1.f), {202, 298} }
};