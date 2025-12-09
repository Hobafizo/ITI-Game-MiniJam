#pragma once
#include "bfObject.h"

class bfRectangle : public bfObject
{
private:
	sf::RectangleShape _shape;
	b2Body* _body;
private:
	b2Vec2 _redirectDirection{ 0,0 }; // default no redirect
	bool _isTransparent = false;    // for preview

public:
	void SetRedirectDirection(const b2Vec2& dir) { _redirectDirection = dir; }
	b2Vec2 RedirectDirection() const { return _redirectDirection; }

	void SetTransparent(bool transparent);
	bool IsTransparent() const { return _isTransparent; }


	bfRectangle(b2Body* body, const sf::Vector2f size, unsigned int color = 0xFFFFFFF);
	virtual ~bfRectangle();

	void setSfPosition(const sf::Vector2f pos) override;
	void setRotation(float angle) override;

	void setOrigin(const sf::Vector2f position);
	void setSize(const sf::Vector2f size);
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