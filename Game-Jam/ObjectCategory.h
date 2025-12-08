#pragma once

enum class ObjectCategory : unsigned short
{
	Player = 1,
	Wall_Vertical = 2,
	Wall_Horizontal = 4,
	SpeedWall_Vertical = 8,
	SpeedWall_Horizontal = 16,
};
