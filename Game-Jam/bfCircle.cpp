#include "bfCircle.h"

bfCircle::bfCircle(b2Body* body, float radius, unsigned int color) : _body(body)
{
	setRadius(radius);
	setOrigin({ radius, radius });
	setFillColor(color);
}

bfCircle::~bfCircle(void)
{
}

void bfCircle::setSfPosition(const sf::Vector2f pos)
{
	_shape.setPosition(pos);
}

void bfCircle::setOrigin(const sf::Vector2f position)
{
	_shape.setOrigin(position);
}

void bfCircle::setRadius(float radius)
{
	_shape.setRadius(radius);
}

void bfCircle::setFillColor(unsigned int color)
{
	setFillColor(sf::Color(color));
}

void bfCircle::setFillColor(const sf::Color color)
{
	_shape.setFillColor(color);
}

void bfCircle::setOutlineColor(unsigned int color)
{
	setOutlineColor(sf::Color(color));
}

void bfCircle::setOutlineColor(const sf::Color color)
{
	_shape.setOutlineColor(color);
}

b2Body* bfCircle::Body(void) const
{
	return _body;
}

sf::Drawable* bfCircle::Drawable(void) const
{
	return (sf::Drawable*)&_shape;
}

b2Vec2 bfCircle::getB2Position(void) const
{
	return _body->GetPosition();
}

sf::Vector2f bfCircle::getSfPosition(void) const
{
	return _shape.getPosition();
}
