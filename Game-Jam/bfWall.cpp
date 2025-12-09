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

bool bfWall::loadSpriteSheet(const std::string& filepath, int frameWidth, int frameHeight, int marginX, int marginY, int framePerLine, int numFrames, float curTime, float frameTime)
{
    if (!_texture.loadFromFile(filepath))
        return false;

    // 1. Enable texture on the circle shape
    _shape.setTexture(&_texture);

    // 2. Setup Animation Data
    _frameSize = { frameWidth, frameHeight };
    _frameMargin = { marginX, marginY };
    _framesPerLine = framePerLine;
    _numFrames = numFrames;
    _currentFrame = 0;
    _frameStartIdx = 0;
    _frameStartTime = curTime;
    _frameDuration = frameTime;

    // 3. Set the initial frame (Rectangle 0)
    _shape.setTextureRect(sf::IntRect(_frameMargin.x, _frameMargin.y, frameWidth, frameHeight));

    // Optional: Reset color to White so it doesn't tint the texture
    _shape.setFillColor(sf::Color::White);

    return true;
}

bool bfWall::updateSpriteSheet(int startIdx, int numFrames, float curTime, float frameTime)
{
    // Update the state
    _frameStartIdx = startIdx;
    _numFrames = numFrames;
    _frameStartTime = curTime; // Reset the timer to NOW
    _frameDuration = frameTime;

    // Force immediate update of the first frame (optional but looks snappier)
    _currentFrame = startIdx;

    int left = (_currentFrame % _framesPerLine) * (_frameSize.x + _frameMargin.x);
    int top = (_currentFrame / _framesPerLine) * (_frameSize.y + _frameMargin.y);

    _shape.setTextureRect(sf::IntRect(left, top, _frameSize.x, _frameSize.y));

    return true;
}

void bfWall::setRotation(float angle)
{
    _body->SetTransform(getB2Position(), angle);
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
    updateAnimation(curTime);
}

void bfWall::updateAnimation(float curTime)
{
    // --- ANIMATION SYNC ---
    if (_numFrames > 0 && _frameDuration > 0.0f)
    {
        // Calculate how much time has passed since this specific animation started
        float elapsed = curTime - _frameStartTime;

        // Calculate how many frames *should* have played in that time
        int totalFramesPassed = (int)(elapsed / _frameDuration);

        // Wrap around using modulo (%) to loop the animation
        int currentOffset = totalFramesPassed % _numFrames;

        // Determine the actual frame index
        _currentFrame = _frameStartIdx + currentOffset;

        // Update the Texture Rectangle
        int left = (_currentFrame % _framesPerLine) * (_frameSize.x + _frameMargin.x);
        int top = (_currentFrame / _framesPerLine) * (_frameSize.y + _frameMargin.y);

        _shape.setTextureRect(sf::IntRect(left, top, _frameSize.x, _frameSize.y));
    }
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
