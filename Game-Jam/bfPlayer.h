#pragma once
#include "bfCharacter.h"

class bfPlayer : public bfCharacter
{
public:
	bfPlayer(b2Body* body, float radius = 0, unsigned int color = 0xFFFFFFF);
	virtual ~bfPlayer();
};