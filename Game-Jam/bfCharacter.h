#pragma once
#include "bfObject.h"
#include "CharacterData.h"

class bfCharacter : public bfObject
{
protected:
	sf::CircleShape _shape;
	b2Body* _body;
	WalkStatus _status;

private:
	float _speedBoostTimer;
	float _boostMultiplier;

public:
	bfCharacter(b2Body* body, float radius = 0, unsigned int color = 0xFFFFFFF);
	virtual ~bfCharacter();

protected:
	void setStatus(const WalkStatus status);

public:
	void setSfPosition(const sf::Vector2f pos) override;

	void setOrigin(const sf::Vector2f position);
	void setRadius(float radius);
	void setFillColor(unsigned int color);
	void setFillColor(const sf::Color color);
	void setOutlineColor(unsigned int color);
	void setOutlineColor(const sf::Color color);
	void setSpeedBoostTimer(float timer);
	void setBoostMultiplier(float multiplier);

	void updatePosition(float curTime) override;

	b2Body* Body() const override;
	sf::Drawable* Drawable() const override;
	b2Vec2 getB2Position() const override;
	sf::Vector2f getSfPosition() const override;
	WalkStatus getStatus() const;
	float getSpeedBoostTimer() const;
	float getBoostMultiplier() const;
};