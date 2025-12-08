#include "bfPlayer.h"

bfPlayer::bfPlayer(b2Body* body, float radius, unsigned int color) : _body(body)
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
