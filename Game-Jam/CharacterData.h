#pragma once

enum class WalkStatus : unsigned char
{
	Idle,
	Moving,
};

enum class WalkDirection : unsigned char
{
	Up,
	Down,
	Left,
	Right,
};

enum class Monster_MovePattern : unsigned char
{
	Unknown,
	Up,
	Down,
	Left,
	Right,
};