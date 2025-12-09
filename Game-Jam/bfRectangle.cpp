#include "bfRectangle.h"

bfRectangle::bfRectangle(b2Body* body, const sf::Vector2f size, unsigned int color) : _body(body)
{
	setSize(size);
	setOrigin({ size.x / 2, size.y / 2 });
	setFillColor(color);
}

bfRectangle::~bfRectangle(void)
{
}

void bfRectangle::setSfPosition(const sf::Vector2f pos)
{
	_shape.setPosition(pos);
}

void bfRectangle::setOrigin(const sf::Vector2f position)
{
	_shape.setOrigin(position);
}

void bfRectangle::setSize(const sf::Vector2f size)
{
	_shape.setSize(size);
}

void bfRectangle::setFillColor(unsigned int color)
{
	setFillColor(sf::Color(color));
}

void bfRectangle::setFillColor(const sf::Color color)
{
	_shape.setFillColor(color);
}

void bfRectangle::setOutlineColor(unsigned int color)
{
	_shape.setOutlineColor(sf::Color(color));
}

void bfRectangle::setOutlineColor(const sf::Color color)
{
	_shape.setOutlineColor(color);
}

void bfRectangle::updatePosition(float curTime)
{

}

b2Body* bfRectangle::Body(void) const
{
	return _body;
}

sf::Drawable* bfRectangle::Drawable(void) const
{
	return (sf::Drawable*)&_shape;
}

b2Vec2 bfRectangle::getB2Position(void) const
{
	return _body->GetPosition();
}

sf::Vector2f bfRectangle::getSfPosition(void) const
{
	return _shape.getPosition();
}
void bfRectangle::SetTransparent(bool transparent)
{
	_isTransparent = transparent;
	sf::Color color = _shape.getFillColor();
	color.a = transparent ? 100 : 255; // semi-transparent or opaque
	_shape.setFillColor(color);
}


