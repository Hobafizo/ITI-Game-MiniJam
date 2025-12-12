//#pragma once
//#include "LevelData.hpp"
//static LevelData Level2Data = {
//
//    // Player position in pixels
//    // (500, 100) means the player starts 500px from the left and 100px from the top.
//    { sf::Vector2f(960.f, 540.f) },
//
//    // Wall data: list of walls with spawn position and size in pixels.
//    // These walls are thin because the sizes are small.
//    // Example: size (100, 2) pixels becomes (10m x 0.2m) after conversion.
//    {
//        { sf::Vector2f(190.f, 100.f),  sf::Vector2f(20.f, 100.f), 0.f },
//        { sf::Vector2f(50.f, 700.f), sf::Vector2f(100.f, 20.f), 0.f },
//        
//    },
//
//    // Enemy data: enemy spawn position and radius in pixels.
//    // radius 1.5f becomes 0.15 meters in Box2D.
//    {
//        { sf::Vector2f(1600.f, 700.f), 20.5f }
//    },
//
//    // Traps (empty for now)
//    {},
//
//    // Key position in pixels
//    { sf::Vector2f(1800.f, 900.f) },
//
//    // Door position in pixels
//    { sf::Vector2f(95.f, 1.f) }
//};