#include "bfPlayer.h"
#include "def.h"

bfPlayer::bfPlayer(b2Body* body, float radius, unsigned int color) : _body(body), _boostMultiplier(1), _speedBoostTimer(0)
{
	setRadius(radius);
	setOrigin({ radius, radius });
	setFillColor(color);
}

bfPlayer::~bfPlayer(void)
{
}

void bfPlayer::setSfPosition(const sf::Vector2f pos)
{
	_shape.setPosition(pos);
}

void bfPlayer::setOrigin(const sf::Vector2f position)
{
	_shape.setOrigin(position);
}

void bfPlayer::setRadius(float radius)
{
	_shape.setRadius(radius);
}

void bfPlayer::setFillColor(unsigned int color)
{
	setFillColor(sf::Color(color));
}

void bfPlayer::setFillColor(const sf::Color color)
{
	_shape.setFillColor(color);
}

void bfPlayer::setOutlineColor(unsigned int color)
{
	setOutlineColor(sf::Color(color));
}

void bfPlayer::setOutlineColor(const sf::Color color)
{
	_shape.setOutlineColor(color);
}

void bfPlayer::setSpeedBoostTimer(float timer)
{
	_speedBoostTimer = timer;
}

void bfPlayer::setBoostMultiplier(float multiplier)
{
	_boostMultiplier = multiplier;
}

void bfPlayer::updateVelocity(float curTime)
{
	float multiplier = curTime > _speedBoostTimer ? 1 : _boostMultiplier;

	b2Vec2 vel = _body->GetLinearVelocity();
	vel.Normalize();

	_body->SetLinearVelocity(PLAYER_SPEED * multiplier * vel);
}

b2Body* bfPlayer::Body(void) const
{
	return _body;
}

sf::Drawable* bfPlayer::Drawable(void) const
{
	return (sf::Drawable*)&_shape;
}

b2Vec2 bfPlayer::getB2Position(void) const
{
	return _body->GetPosition();
}

sf::Vector2f bfPlayer::getSfPosition(void) const
{
	return _shape.getPosition();
}

float bfPlayer::getSpeedBoostTimer(void) const
{
	return _speedBoostTimer;
}

float bfPlayer::getBoostMultiplier(void) const
{
	return _boostMultiplier;
}
