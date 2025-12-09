#include "bfWall.h"

bfWall::bfWall(b2Body* body, const sf::Vector2f size, unsigned int color) : _body(body)
{
	setSize(size);
	setOrigin({ size.x / 2, size.y / 2 });
	setFillColor(color);
}

bfWall::~bfWall(void)
{
}

void bfWall::setSfPosition(const sf::Vector2f pos)
{
	_shape.setPosition(pos);
}

void bfWall::setOrigin(const sf::Vector2f position)
{
	_shape.setOrigin(position);
}

void bfWall::setSize(const sf::Vector2f size)
{
	_shape.setSize(size);
}

void bfWall::setFillColor(unsigned int color)
{
	setFillColor(sf::Color(color));
}

void bfWall::setFillColor(const sf::Color color)
{
	_shape.setFillColor(color);
}

void bfWall::setOutlineColor(unsigned int color)
{
	setOutlineColor(sf::Color(color));
}

void bfWall::setOutlineColor(const sf::Color color)
{
	_shape.setOutlineColor(color);
}

void bfWall::updatePosition(float curTime)
{

}

b2Body* bfWall::Body(void) const
{
	return _body;
}

sf::Drawable* bfWall::Drawable(void) const
{
	return (sf::Drawable*)&_shape;
}

b2Vec2 bfWall::getB2Position(void) const
{
	return _body->GetPosition();
}

sf::Vector2f bfWall::getSfPosition(void) const
{
	return _shape.getPosition();
}
