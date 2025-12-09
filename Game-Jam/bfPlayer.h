#pragma once
#include "bfCharacter.h"

class bfPlayer : public bfCharacter
{
public:
	bfPlayer(b2Body* body, const sf::Vector2f size, unsigned int color = 0xFFFFFFF);
	virtual ~bfPlayer();

	void updatePosition(float curTime) override;
	void updateAnimation(float curTime) override;
};