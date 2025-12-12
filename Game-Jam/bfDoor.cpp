#include "bfDoor.h"
#include "def.h"

bfDoor::bfDoor(b2Body* body, const sf::Vector2f size, unsigned int color) : _body(body)
{
	setSize(size);
	setOrigin({ size.x / 2, size.y / 2 });
	setFillColor(color);
}

bfDoor::~bfDoor(void)
{
}

bool bfDoor::loadSpriteSheet(const std::string& filepath, int frameWidth, int frameHeight, int marginX, int marginY, int framePerLine, int numFrames, float curTime, float frameTime, bool autoResize)
{
	autoResize = false;

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

    if (autoResize)
        resizeToFitFrame();

    return true;
}

bool bfDoor::updateSpriteSheet(int startIdx, int numFrames, float curTime, float frameTime)
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

void bfDoor::resizeToFitFrame()
{
	// 1. Get current base size
	sf::Vector2f currentSize = (sf::Vector2f)_texture.getSize();
	sf::Vector2f targetSize = _shape.getSize();

	// 2. Safety: If size is 0, set it directly instead of scaling
	if (currentSize.x < 0.1f || currentSize.y < 0.1f)
	{
		sf::Vector2f newSize(targetSize.x, targetSize.y);
		setSize(newSize);
		setOrigin({ newSize.x / 2.0f, newSize.y / 2.0f });
		setScale({ 1.0f, 1.0f });
	}
	else
	{
		// 3. Calculate Scale Ratio (Target / Current)
		sf::Vector2f newScale;
		newScale.x = targetSize.x / currentSize.x;
		newScale.y = targetSize.y / currentSize.y;

		// 4. Apply via setScale (Triggers Physics Update)
		setScale(newScale);
	}
}

void bfDoor::setRotation(float angle)
{
    _body->SetTransform(getB2Position(), angle);
}

void bfDoor::setScale(const sf::Vector2f scale)
{
    _shape.setScale(scale);

    if (_body)
    {
        // Calculate the "Real" size in pixels: (Base Size * Scale Factor)
        sf::Vector2f baseSize = _shape.getSize();
        float scaledWidth = baseSize.x * scale.x;
        float scaledHeight = baseSize.y * scale.y;

        // Convert to Box2D Half-Extents (Meters)
        // Box2D uses half-width and half-height for boxes
        float hx = (scaledWidth / 2.0f) / PIXELS_PER_UNIT;
        float hy = (scaledHeight / 2.0f) / PIXELS_PER_UNIT;

        // Iterate through fixtures to find and resize the box shape
        for (b2Fixture* f = _body->GetFixtureList(); f; f = f->GetNext())
        {
            b2Shape* shape = f->GetShape();

            // We only want to resize the polygon (box) shape
            if (shape->GetType() == b2Shape::e_polygon)
            {
                b2PolygonShape* poly = (b2PolygonShape*)shape;

                // Resize the shape geometry
                // This assumes the shape is centered at (0,0) relative to the body
                poly->SetAsBox(hx, hy);
            }
        }

        // 3. IMPORTANT: Recalculate Mass
        // Changing geometry changes mass/inertia. If you skip this, physics feels "floaty".
        _body->ResetMassData();

        // Wake the body so it reacts to the size change immediately
        _body->SetAwake(true);
    }
}

void bfDoor::setSfPosition(const sf::Vector2f pos)
{
	_shape.setPosition(pos);
}

void bfDoor::setPositionFromTopLeft(const sf::Vector2f& topLeft)
{
    sf::Vector2f size = _shape.getSize();

    // Calculate center
    sf::Vector2f centerPos = topLeft + (size * 0.5f);

    // Use your existing position setter (handles Box2D sync)
    setSfPosition(centerPos);
}

void bfDoor::setOrigin(const sf::Vector2f position)
{
	_shape.setOrigin(position);
}

void bfDoor::setSize(const sf::Vector2f size)
{
	_shape.setSize(size);
}

void bfDoor::setFillColor(unsigned int color)
{
	setFillColor(sf::Color(color));
}

void bfDoor::setFillColor(const sf::Color color)
{
	_shape.setFillColor(color);
}

void bfDoor::setOutlineColor(unsigned int color)
{
	setOutlineColor(sf::Color(color));
}

void bfDoor::setOutlineColor(const sf::Color color)
{
	_shape.setOutlineColor(color);
}

void bfDoor::updatePosition(float curTime)
{
    updateAnimation(curTime);
}

void bfDoor::updateAnimation(float curTime)
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

b2Body* bfDoor::Body(void) const
{
	return _body;
}

sf::Drawable* bfDoor::Drawable(void) const
{
	return (sf::Drawable*)&_shape;
}

b2Vec2 bfDoor::getB2Position(void) const
{
	return _body->GetPosition();
}

sf::Vector2f bfDoor::getSfPosition(void) const
{
	return _shape.getPosition();
}
