#include "bfCharacter.h"
#include "def.h"

bfCharacter::bfCharacter(b2Body* body, float radius, unsigned int color) : _body(body), _boostMultiplier(1), _speedBoostTimer(0)
{
	setRadius(radius);
	setOrigin({ radius, radius });
	setFillColor(color);
}

bfCharacter::~bfCharacter(void)
{
}

void bfCharacter::setStatus(const WalkStatus status)
{
	_status = status;
}

void bfCharacter::setSfPosition(const sf::Vector2f pos)
{
	_shape.setPosition(pos);
}

void bfCharacter::setOrigin(const sf::Vector2f position)
{
	_shape.setOrigin(position);
}

void bfCharacter::setRadius(float radius)
{
	_shape.setRadius(radius);
}

void bfCharacter::setFillColor(unsigned int color)
{
	setFillColor(sf::Color(color));
}

void bfCharacter::setFillColor(const sf::Color color)
{
	_shape.setFillColor(color);
}

void bfCharacter::setOutlineColor(unsigned int color)
{
	setOutlineColor(sf::Color(color));
}

void bfCharacter::setOutlineColor(const sf::Color color)
{
	_shape.setOutlineColor(color);
}

void bfCharacter::setSpeedBoostTimer(float timer)
{
	_speedBoostTimer = timer;
}

void bfCharacter::setBoostMultiplier(float multiplier)
{
	_boostMultiplier = multiplier;
}

void bfCharacter::updatePosition(float curTime)
{
	float multiplier = curTime > _speedBoostTimer ? 1 : _boostMultiplier;

	b2Vec2 vel = _body->GetLinearVelocity();
	vel.Normalize();

	_body->SetLinearVelocity(PLAYER_SPEED * multiplier * vel);
}

b2Body* bfCharacter::Body(void) const
{
	return _body;
}

sf::Drawable* bfCharacter::Drawable(void) const
{
	return (sf::Drawable*)&_shape;
}

b2Vec2 bfCharacter::getB2Position(void) const
{
	return _body->GetPosition();
}

sf::Vector2f bfCharacter::getSfPosition(void) const
{
	return _shape.getPosition();
}

WalkStatus bfCharacter::getStatus(void) const
{
	return _status;
}

float bfCharacter::getSpeedBoostTimer(void) const
{
	return _speedBoostTimer;
}

float bfCharacter::getBoostMultiplier(void) const
{
	return _boostMultiplier;
}
