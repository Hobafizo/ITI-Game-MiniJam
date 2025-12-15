#pragma once
#include "LevelData.hpp"
#include "ObjectCategory.h"
#include<SFML/Audio.hpp>

static LevelData Level5Data = {
	//player pos
	{
	sf::Vector2f(100.f, 500.f),
	},
	//walls and positions

	{
		{sf::Vector2f(500,800),sf::Vector2f(WALL_VERTICAL_WIDTH, WALL_VERTICAL_HEIGHT),ObjectCategory::Wall_Vertical},
		{sf::Vector2f(700,600),sf::Vector2f(WALL_HORIZONTAL_WIDTH, WALL_HORIZONTAL_HEIGHT),ObjectCategory::Wall_Horizontal},
		{sf::Vector2f(900,400),sf::Vector2f(WALL_VERTICAL_WIDTH, WALL_VERTICAL_HEIGHT),ObjectCategory::Wall_Vertical},
	},

	//enemies and positions
	{
		{sf::Vector2f(600.f, 500.f), { 108, 76 } , 1},
		{sf::Vector2f(800.f, 300.f), {83, 87} , 2},
		{sf::Vector2f(1200,700), { 94, 56},3},
	},
	//empty traps
	{},

	//key
	
	{
		sf::Vector2f(1300.f, 300.f),
		{ KEY_WIDTH, KEY_HEIGHT },
	},
	//door
	{
		sf::Vector2f(1400.f, 500.f),
		{ GATE_WIDTH, GATE_HEIGHT},
	},
};