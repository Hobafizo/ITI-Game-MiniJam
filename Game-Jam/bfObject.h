#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "ObjectCategory.h"
class bfObject
{
protected:
	bfObject();
	b2Vec2 _redirectDirection{ 0,0 }; // default no redirect
	bool _isTransparent = false;    // for preview

public:
	void SetRedirectDirection(const b2Vec2& dir) { _redirectDirection = dir; }
	b2Vec2 RedirectDirection() const { return _redirectDirection; }

	void SetTransparent(bool val) { _isTransparent = val; }
	bool IsTransparent() const { return _isTransparent; }

	virtual ~bfObject();

	virtual void setSfPosition(const sf::Vector2f pos) = 0;
	virtual void updatePosition(float curTime) = 0;

	virtual b2Body* Body() const = 0;
	virtual sf::Drawable* Drawable() const = 0;
	virtual b2Vec2 getB2Position() const = 0;
	virtual sf::Vector2f getSfPosition() const = 0;
	virtual ObjectCategory Category() const { return ObjectCategory::Wall; } // default, override in subclasses

};