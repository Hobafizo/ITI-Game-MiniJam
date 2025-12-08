#pragma once

enum class ObjectCategory : unsigned short
{
	Player = 1,
	Wall = 2,
	SpeedWall = 4,
	PlayerWall = 8,
	PlayerWallRedirect = 16
};
