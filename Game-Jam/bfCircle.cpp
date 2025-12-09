#include "bfCircle.h"

bfCircle::bfCircle(b2Body* body, float radius, unsigned int color) : _body(body)
{
	setRadius(radius);
	setOrigin({ radius, radius });
	setFillColor(color);
}

bfCircle::~bfCircle(void)
{
}

bool bfCircle::loadSpriteSheet(const std::string& filepath, int frameWidth, int frameHeight, int numFrames, float curTime, float frameTime)
{
    if (!_texture.loadFromFile(filepath))
        return false;

    // 1. Enable texture on the circle shape
    _shape.setTexture(&_texture);

    // 2. Setup Animation Data
    _frameSize = sf::Vector2i(frameWidth, frameHeight);
    _numFrames = numFrames;
    _currentFrame = 0;
    _frameStartIdx = 0;
    _frameStartTime = curTime;
    _frameDuration = frameTime;

    // 3. Set the initial frame (Rectangle 0)
    _shape.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));

    // Optional: Reset color to White so it doesn't tint the texture
    _shape.setFillColor(sf::Color::White);

    return true;
}

bool bfCircle::updateSpriteSheet(int startIdx, int numFrames, float curTime, float frameTime)
{
    // Update the state
    _frameStartIdx = startIdx;
    _numFrames = numFrames;
    _frameStartTime = curTime; // Reset the timer to NOW
    _frameDuration = frameTime;

    // Force immediate update of the first frame (optional but looks snappier)
    _currentFrame = startIdx;
    int left = _currentFrame * _frameSize.x;
    _shape.setTextureRect(sf::IntRect(left, 0, _frameSize.x, _frameSize.y));

    return true;
}

void bfCircle::setSfPosition(const sf::Vector2f pos)
{
	_shape.setPosition(pos);
}

void bfCircle::setRotation(float angle)
{
    _body->SetTransform(getB2Position(), angle);
}

void bfCircle::setOrigin(const sf::Vector2f position)
{
	_shape.setOrigin(position);
}

void bfCircle::setRadius(float radius)
{
	_shape.setRadius(radius);
}

void bfCircle::setFillColor(unsigned int color)
{
	setFillColor(sf::Color(color));
}

void bfCircle::setFillColor(const sf::Color color)
{
	_shape.setFillColor(color);
}

void bfCircle::setOutlineColor(unsigned int color)
{
	setOutlineColor(sf::Color(color));
}

void bfCircle::setOutlineColor(const sf::Color color)
{
	_shape.setOutlineColor(color);
}

void bfCircle::updatePosition(float curTime)
{
    // --- ANIMATION SYNC ---
    if (_numFrames > 1 && _frameDuration > 0.0f)
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
        int left = _currentFrame * _frameSize.x;
        int top = 0; // Assuming single row. If multiple rows, calculate 'top' here.

        _shape.setTextureRect(sf::IntRect(left, top, _frameSize.x, _frameSize.y));
    }
}

b2Body* bfCircle::Body(void) const
{
	return _body;
}

sf::Drawable* bfCircle::Drawable(void) const
{
	return (sf::Drawable*)&_shape;
}

b2Vec2 bfCircle::getB2Position(void) const
{
	return _body->GetPosition();
}

sf::Vector2f bfCircle::getSfPosition(void) const
{
	return _shape.getPosition();
}
