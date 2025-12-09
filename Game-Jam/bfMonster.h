#pragma once
#include "bfCharacter.h"
#include "CharacterData.h"

class bfMonster : public bfCharacter
{
private:
	Monster_MovePattern _movePattern;

public:
	bfMonster(b2Body* body, float radius = 0, unsigned int color = 0xFFFFFFF);
	virtual ~bfMonster();

public:
	void setMovePattern(Monster_MovePattern pattern);
	
	void updatePosition(float curTime) override;

	Monster_MovePattern getMovementPattern() const;
	Monster_MovePattern toggleMovementPattern();
};