#include "bfPlayer.h"

bfPlayer::bfPlayer(b2Body* body, const sf::Vector2f size, unsigned int color) : bfCharacter(body, size, color)
{
}

bfPlayer::~bfPlayer(void)
{
}

void bfPlayer::updatePosition(float curTime)
{
	bfCharacter::updatePosition(curTime);
	//updateAnimation(curTime);
}

void bfPlayer::updateAnimation(float curTime)
{
	b2Body* body = Body();
	b2Vec2 velocity = body->GetLinearVelocity();

	int frameStartIdx = 0;
	int frameCount = 1;
	int frameDuration = 1;

	WalkDirection dir = getWalkDirection(velocity);

	switch (dir)
	{
	case WalkDirection::Up:
		frameStartIdx = 0;
		frameCount = 1;
		break;

	case WalkDirection::Down:
		frameStartIdx = 1;
		frameCount = 1;
		break;

	case WalkDirection::Left:
		frameStartIdx = 2;
		frameCount = 1;
		break;

	case WalkDirection::Right:
		frameStartIdx = 3;
		frameCount = 1;
		break;
	}

	updateSpriteSheet(frameStartIdx, frameCount, curTime, frameDuration);
}
