#pragma once
#include "LevelData.hpp"
#include "ObjectCategory.h"
#include<SFML/Audio.hpp>

static LevelData Level4Data = {

	// Player position in pixels
	// (500, 100) means the player starts 500px from the left and 100px from the top.
	{ sf::Vector2f(200.f, 900.f) },

	{
		// Vertical wall (you already had this one correct)
		{ sf::Vector2f(600.f, 800.f),
			   sf::Vector2f(WALL_VERTICAL_WIDTH, WALL_VERTICAL_HEIGHT),

				ObjectCategory::Wall_Vertical },
				//hor wall
				{ sf::Vector2f(900.f, 600.f),
				  sf::Vector2f(WALL_HORIZONTAL_WIDTH, WALL_HORIZONTAL_HEIGHT),
				  ObjectCategory::Wall_Horizontal},

				  // Vert wall
				  { sf::Vector2f(1200.f, 800.f),
					sf::Vector2f(WALL_VERTICAL_WIDTH,WALL_VERTICAL_HEIGHT),

					ObjectCategory::Wall_Vertical },

			  },
			  // Enemy data: enemy spawn position and radius in pixels.
	{

	{
		sf::Vector2f(400.f, 700.f),{ 108, 76 } , 1},

		{ sf::Vector2f(1400.f, 900.f), { 83, 87 }, 2  },

		//{ sf::Vector2f(1200.f, 500.f), { 94, 56 }, 3  },
		{ sf::Vector2f(800.f, 900.f),{ 108, 76 } , 1},
		{ sf::Vector2f(1300.f, 700.f), { 83, 87 }, 2  },

		{ sf::Vector2f(1300.f, 500.f), { 94, 56 }, 3  }
	},
	// Traps (empty for now)
	{},
	// Key position in pixels
	{ sf::Vector2f(1300.f, 800.f),{ KEY_WIDTH, KEY_HEIGHT } },
	// Door position in pixels
	{ sf::Vector2f(1300.f, 100.f),{ GATE_WIDTH, GATE_HEIGHT } }

};