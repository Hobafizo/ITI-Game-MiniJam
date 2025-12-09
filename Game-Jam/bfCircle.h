#pragma once
#include "bfObject.h"

class bfCircle : public bfObject
{
private:
	sf::CircleShape _shape;
	b2Body* _body;

public:
	bfCircle(b2Body* body, float radius = 0, unsigned int color = 0xFFFFFFF);
	virtual ~bfCircle();

	void setSfPosition(const sf::Vector2f pos) override;

	void setOrigin(const sf::Vector2f position);
	void setRadius(float radius);
	void setFillColor(unsigned int color);
	void setFillColor(const sf::Color color);
	void setOutlineColor(unsigned int color);
	void setOutlineColor(const sf::Color color);

	void updatePosition(float curTime) override;

	b2Body* Body() const override;
	sf::Drawable* Drawable() const override;
	b2Vec2 getB2Position() const override;
	sf::Vector2f getSfPosition() const override;
};