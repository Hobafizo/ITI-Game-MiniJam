#pragma once

enum class ObjectCategory : unsigned short
{
	None = 0,
	Player = 1,
	Wall_Vertical = 2,
	Wall_Horizontal = 4,
	SpeedWall_Vertical = 8,
	SpeedWall_Horizontal = 16,
	Monster = 32,
	Wall = 64,
	SpeedWall = 128,
	Key = 256,
	Gate = 512,
	LWall = 1024,
	LWall_Left = 2048,
	LWall_Bottom = 4096,
};
