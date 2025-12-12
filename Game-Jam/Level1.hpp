//#pragma once
//#include "LevelData.hpp"
//#include "ObjectCategory.h"
///*
//    This file defines Level1Data using simple ASCII characters only.
//
//    IMPORTANT NOTES:
//    LevelData uses pixel units because SFML works in pixels.
//    Box2D uses meters, so LevelManager converts:
//        pixels -> meters
//    using PIXELS_PER_UNIT.
//
//    Example:
//        PIXELS_PER_UNIT = 10
//        10 pixels = 1 meter
//*/
//
//static LevelData Level1Data = {
//
//    // Player position in pixels
//    // (500, 100) means the player starts 500px from the left and 100px from the top.
//    { sf::Vector2f(500, 100.f) },
//
//    // Wall data: list of walls with spawn position and size in pixels.
//    // These walls are thin because the sizes are small.
//    // Example: size (100, 2) pixels becomes (10m x 0.2m) after conversion.
//    {
//        { sf::Vector2f(960.f, 200.f),  sf::Vector2f(20.f, 200.f), 0.f, },
//        { sf::Vector2f(500.f, 540.f), sf::Vector2f(WALL_VERTICAL_WIDTH, WALL_VERTICAL_HEIGHT), 0.f },
//        //{ sf::Vector2f(500, 100.f) , sf::Vector2f(50.f,50.f),0.f},
//
//    },
//
//    // Enemy data: enemy spawn position and radius in pixels.
//    // radius 1.5f becomes 0.15 meters in Box2D.
//    {
//        /*{ sf::Vector2f(50.f, 30.f), 1.5f }*/
//    },
//
//    // Traps (empty for now)
//    {},
//
//    // Key position in pixels
//    { sf::Vector2f(1800.f, 650.f) },
//
//    // Door position in pixels
//    { sf::Vector2f(1800.f, 100.f) }
//};