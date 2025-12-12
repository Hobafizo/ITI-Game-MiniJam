#include "bfCircle.h"
#include "def.h"

bfCircle::bfCircle(b2Body* body, float radius, unsigned int color) : _body(body)
{
	setRadius(radius);
	setOrigin({ radius, radius });
	setFillColor(color);
}

bfCircle::~bfCircle(void)
{
}

bool bfCircle::loadSpriteSheet(const std::string& filepath, int frameWidth, int frameHeight, int marginX, int marginY, int framePerLine, int numFrames, float curTime, float frameTime, bool autoResize)
{
	autoResize = false;

	if (!_texture.loadFromFile(filepath))
		return false;

	// 1. Enable texture on the circle shape
	_shape.setTexture(&_texture);

	// 2. Setup Animation Data
	_frameSize = (sf::Vector2i)_texture.getSize();
	_frameMargin = { marginX, marginY };
	_framesPerLine = framePerLine;
	_numFrames = numFrames;
	_currentFrame = 0;
	_frameStartIdx = 0;
	_frameStartTime = curTime;
	_frameDuration = frameTime;

	// 3. Set the initial frame (Rectangle 0)
	_shape.setTextureRect(sf::IntRect(_frameMargin.x, _frameMargin.y, _frameSize.x, _frameSize.y));

	// Optional: Reset color to White so it doesn't tint the texture
	_shape.setFillColor(sf::Color::White);

    return true;
}

bool bfCircle::updateSpriteSheet(int startIdx, int numFrames, float curTime, float frameTime)
{
	if (!_framesPerLine || !numFrames)
		return false;

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

void bfCircle::setSfPosition(const sf::Vector2f pos)
{
	_shape.setPosition(pos);
}

void bfCircle::setPositionFromTopLeft(const sf::Vector2f& topLeft)
{
    // 1. Get the current visual radius
    float r = _shape.getRadius();

    // 2. Account for Scale (just in case the sprite was scaled up/down)
    sf::Vector2f scale = _shape.getScale();
    float effectiveRadiusX = r * scale.x;
    float effectiveRadiusY = r * scale.y;

    // 3. Calculate Center
    // Center = TopLeft + Radius
    sf::Vector2f centerPos;
    centerPos.x = topLeft.x + effectiveRadiusX;
    centerPos.y = topLeft.y + effectiveRadiusY;

    // 4. Apply using your existing sync function
    setSfPosition(centerPos);
}

void bfCircle::setRotation(float angle)
{
    _body->SetTransform(getB2Position(), angle);
}

void bfCircle::setScale(const sf::Vector2f scale)
{
    // 1. Update Visuals (SFML)
    _shape.setScale(scale);

    // 2. Update Physics (Box2D)
    if (_body)
    {
        // Calculate the new visual radius based on the X scale 
        // (Box2D circles cannot be ovals, so we must pick one axis, usually X)
        float currentRadius = _shape.getRadius();
        float scaledRadius = currentRadius * scale.x;

        // Convert to Meters
        float radiusMeters = scaledRadius / PIXELS_PER_UNIT;

        // Iterate through fixtures
        for (b2Fixture* f = _body->GetFixtureList(); f; f = f->GetNext())
        {
            b2Shape* shape = f->GetShape();

            // Check if this fixture is actually a CIRCLE
            if (shape->GetType() == b2Shape::e_circle)
            {
                b2CircleShape* circle = (b2CircleShape*)shape;

                // Update the physics radius directly
                circle->m_radius = radiusMeters;
            }
        }

        // 3. Recalculate Mass
        _body->ResetMassData();
        _body->SetAwake(true);
    }
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
    
	updateAnimation(curTime);
}

void bfCircle::updateAnimation(float curTime)
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
