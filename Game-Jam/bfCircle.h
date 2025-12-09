#pragma once
#include "bfObject.h"

class bfCircle : public bfObject
{
private:
	sf::CircleShape _shape;
	b2Body* _body;
	sf::Texture _texture; // Keeps the image data alive
	sf::Vector2i _frameSize; // Width/Height of one frame
	int _frameStartIdx, _numFrames; // Total frames in the sheet
	int _currentFrame; // Current index
	float _frameStartTime, _frameDuration; // Time per frame

public:
	bfCircle(b2Body* body, float radius = 0, unsigned int color = 0xFFFFFFF);
	virtual ~bfCircle();

	bool loadSpriteSheet(const std::string& filepath, int frameWidth, int frameHeight, int numFrames, float curTime, float frameTime = 0.1f);
	bool updateSpriteSheet(int startIdx, int numFrames, float curTime, float frameTime = 0.1f);

	void setSfPosition(const sf::Vector2f pos) override;
	void setRotation(float angle) override;

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