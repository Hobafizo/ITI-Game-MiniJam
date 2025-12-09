#pragma once
#include "bfWall.h"

class bfSpeedWall : public bfWall
{
public:
	bfSpeedWall(b2Body* body, const sf::Vector2f size, unsigned int color = 0xFFFFFFF);
	virtual ~bfSpeedWall();
};