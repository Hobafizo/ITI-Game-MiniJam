#pragma once
#include "LevelData.hpp"
#include "ObjectCategory.h"

static LevelData Level2Data = {

    // Player
    { sf::Vector2f(960.f, 540.f) },

    // Walls (converted to Level3 format)
    {
        { sf::Vector2f(890.f, 250.f),  sf::Vector2f(WALL_VERTICAL_WIDTH, WALL_VERTICAL_HEIGHT),  ObjectCategory::Wall_Vertical },
        { sf::Vector2f(650.f, 700.f),   sf::Vector2f(WALL_HORIZONTAL_WIDTH, WALL_HORIZONTAL_HEIGHT),  ObjectCategory::Wall_Horizontal },
    },

    // Enemies (converted: give size + type)
    {
        { sf::Vector2f(1600.f, 700.f),{ 108, 76 }, 1 }
    },

    // Traps
    {},

    // Key (added missing size)
    { sf::Vector2f(1800.f, 900.f), {71, 82} },

    // Door (added missing size)
    { sf::Vector2f(550.f, 100.f), {202, 298} }
};