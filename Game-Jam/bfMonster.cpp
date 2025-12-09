#include "bfMonster.h"
#include "def.h"

bfMonster::bfMonster(b2Body* body, float radius, unsigned int color) : bfCharacter(body, radius, color)
{
	setRadius(radius);
	setOrigin({ radius, radius });
	setFillColor(color);
}

bfMonster::~bfMonster(void)
{
}

void bfMonster::setMovePattern(Monster_MovePattern pattern)
{
	_movePattern = pattern;
}

void bfMonster::updatePosition(float curTime)
{
	bfCharacter::updatePosition(curTime);

	if (_movePattern == Monster_MovePattern::Up)
		_body->SetLinearVelocity({ 0, -(PLAYER_SPEED / 2) });
	else if (_movePattern == Monster_MovePattern::Down)
		_body->SetLinearVelocity({ 0, PLAYER_SPEED / 2 });
	else if (_movePattern == Monster_MovePattern::Left)
		_body->SetLinearVelocity({ -(PLAYER_SPEED / 2), 0 });
	else if (_movePattern == Monster_MovePattern::Right)
		_body->SetLinearVelocity({ PLAYER_SPEED / 2, 0 });
}

Monster_MovePattern bfMonster::getMovementPattern(void) const
{
	return _movePattern;
}

Monster_MovePattern bfMonster::toggleMovementPattern(void)
{
	switch (_movePattern)
	{
	case Monster_MovePattern::Up:
		return Monster_MovePattern::Down;

	case Monster_MovePattern::Down:
		return Monster_MovePattern::Up;

	case Monster_MovePattern::Left:
		return Monster_MovePattern::Right;

	case Monster_MovePattern::Right:
		return Monster_MovePattern::Left;
	}

	return Monster_MovePattern::Unknown;
}
