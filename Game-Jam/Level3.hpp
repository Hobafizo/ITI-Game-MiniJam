#pragma once
#include "LevelData.hpp"
#include "ObjectCategory.h"
static LevelData Level3Data = {

    // Player position in pixels
    // (500, 100) means the player starts 500px from the left and 100px from the top.
    { sf::Vector2f(960.f, 900.f) },


     //wall = CreateWall(b2_staticBody, pixelToMeter({ PLAYGROUND_MARGIN_LEFT - WALL_VERTICAL_WIDTH, 0 }), { WALL_VERTICAL_WIDTH, (float)_screenHeight }, 0.01f, 0.3f, (uint16)ObjectCategory::Wall_Vertical, 0, true, false, true);
	//wall = CreateWall(b2_staticBody, pixelToMeter({ (float)_screenWidth - PLAYGROUND_MARGIN_RIGHT, 0 }), { WALL_VERTICAL_WIDTH, (float)_screenHeight }, 0.01f, 0.3f, (uint16)ObjectCategory::Wall_Vertical, 0, true, false, true);
	//wall = CreateWall(b2_staticBody, pixelToMeter({ 0, PLAYGROUND_MARGIN_TOP - WALL_VERTICAL_WIDTH }), { (float)_screenWidth, WALL_VERTICAL_WIDTH }, 0.01f, 0.3f, (uint16)ObjectCategory::Wall_Horizontal, 0, true, false, true);
	//wall = CreateWall(b2_staticBody, pixelToMeter({ 0, (float)_screenHeight - PLAYGROUND_MARGIN_BOTTOM }), { (float)_screenWidth, WALL_VERTICAL_WIDTH }, 0.01f, 0.3f, (uint16)ObjectCategory::Wall_Horizontal, 0, true, false, true);
    
    //wall takes: pos, size, Type
    {
    // Vertical wall (you already had this one correct)
    { sf::Vector2f(960.f, 700.f),
      sf::Vector2f(WALL_VERTICAL_WIDTH,WALL_VERTICAL_HEIGHT),
      ObjectCategory::Wall_Vertical },

      { sf::Vector2f(960.f, 200.f),
        sf::Vector2f(WALL_VERTICAL_HEIGHT,WALL_VERTICAL_WIDTH),
        ObjectCategory::Wall_Vertical},

        
        { sf::Vector2f(910.f, 200.f),
          sf::Vector2f(WALL_VERTICAL_WIDTH,WALL_VERTICAL_HEIGHT),

          ObjectCategory::Wall_Vertical },

          { sf::Vector2f(100.f, 300.f),
            sf::Vector2f(WALL_VERTICAL_WIDTH, WALL_VERTICAL_HEIGHT),

            ObjectCategory::Wall_Vertical },
     },
    // Enemy data: enemy spawn position and radius in pixels.
    /*
    monster = CreateMonster(b2_dynamicBody, pixelToMeter({ PLAYGROUND_MARGIN_LEFT + 600, PLAYGROUND_MARGIN_TOP + 50 }), { 108, 76 }, 0.01f, 0.3f, 1);
    */

    //1 up
    //2 right
    //3 unknown
    {
        { sf::Vector2f(1300.f, 700.f),{ 108, 76 } , 1},
        // EnemyData object 2
        { sf::Vector2f(20.f, 900.f), { 83, 87 }, 2  },

        // EnemyData object 3
		{ sf::Vector2f(1700.f, 500.f), { 94, 56 }, 3  }
    },

    // Traps (empty for now)
    {},

    // Key position in pixels
    { sf::Vector2f(1800.f, 900.f),{ 71, 82 } },

    // Door position in pixels
    { sf::Vector2f(960.f, 20.f),{ 202, 298 } }
};