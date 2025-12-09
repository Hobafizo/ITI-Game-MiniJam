#pragma once
#include "bfObject.h"

class bfWall : public bfObject
{
protected:
	sf::RectangleShape _shape;
	b2Body* _body;

	sf::Texture _texture; // Keeps the image data alive
	sf::Vector2i _frameSize, _frameMargin; // Width/Height of one frame
	int _frameStartIdx, _numFrames, _framesPerLine; // Total frames in the sheet
	int _currentFrame; // Current index
	float _frameStartTime, _frameDuration; // Time per frame

public:
	bfWall(b2Body* body, const sf::Vector2f size, unsigned int color = 0xFFFFFFF);
	virtual ~bfWall();

	bool loadSpriteSheet(const std::string& filepath, int frameWidth, int frameHeight, int marginX, int marginY, int framePerLine, int numFrames, float curTime, float frameTime = 0.1f);
	bool updateSpriteSheet(int startIdx, int numFrames, float curTime, float frameTime = 0.1f);

	void setSfPosition(const sf::Vector2f pos) override;
	void setRotation(float angle) override;

	void setOrigin(const sf::Vector2f position);
	void setSize(const sf::Vector2f size);
	void setFillColor(unsigned int color);
	void setFillColor(const sf::Color color);
	void setOutlineColor(unsigned int color);
	void setOutlineColor(const sf::Color color);

	void updatePosition(float curTime) override;
	virtual void updateAnimation(float curTime);

	b2Body* Body() const override;
	sf::Drawable* Drawable() const override;
	b2Vec2 getB2Position() const override;
	sf::Vector2f getSfPosition() const override;
};