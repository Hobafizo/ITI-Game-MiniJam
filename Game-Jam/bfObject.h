#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class bfObject
{
protected:
	bfObject();
public:
	virtual ~bfObject();

	virtual void setSfPosition(const sf::Vector2f pos) = 0;
	virtual void updatePosition(float curTime) = 0;

	virtual b2Body* Body() const = 0;
	virtual sf::Drawable* Drawable() const = 0;
	virtual b2Vec2 getB2Position() const = 0;
	virtual sf::Vector2f getSfPosition() const = 0;
};