#include "bfCharacter.h"
#include "def.h"

bfCharacter::bfCharacter(b2Body* body, const sf::Vector2f size, unsigned int color) : _body(body), _boostMultiplier(1), _speedBoostTimer(0)
{
    setSize(size);
    setOrigin({ size.x / 2, size.y / 2 });
    setFillColor(color);
}

bfCharacter::~bfCharacter(void)
{
}

void bfCharacter::setStatus(const WalkStatus status)
{
	_status = status;
}

bool bfCharacter::loadSpriteSheet(const std::string& filepath, int frameWidth, int frameHeight, int marginX, int marginY, int framePerLine, int numFrames, float curTime, float frameTime, bool autoResize)
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

    if (autoResize)
        resizeToFitFrame();

    return true;
}

bool bfCharacter::updateSpriteSheet(int startIdx, int numFrames, float curTime, float frameTime)
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

void bfCharacter::resizeToFitFrame()
{
    // 1. Get current base size
    sf::Vector2f currentSize = (sf::Vector2f)_texture.getSize();
    sf::Vector2f textureSize = _shape.getSize();

    // 2. Safety: If size is 0, set it directly instead of scaling
    if (currentSize.x < 0.1f || currentSize.y < 0.1f)
    {
        sf::Vector2f newSize(textureSize.x, textureSize.y);
        setSize(newSize);
        setOrigin({ newSize.x / 2.0f, newSize.y / 2.0f });
        setScale({ 1.0f, 1.0f });
    }
    else
    {
        // 3. Calculate Scale Ratio (Target / Current)
        sf::Vector2f newScale;
        newScale.x = textureSize.x / currentSize.x;
        newScale.y = textureSize.y / currentSize.y;

        // 4. Apply via setScale (Triggers Physics Update)
        setScale(newScale);
    }
}

void bfCharacter::setSfPosition(const sf::Vector2f pos)
{
	_shape.setPosition(pos);
}

void bfCharacter::setPositionFromTopLeft(const sf::Vector2f& topLeft)
{
    sf::Vector2f size = _shape.getSize();

    // Calculate center
    sf::Vector2f centerPos = topLeft + (size * 0.5f);

    // Use your existing position setter (handles Box2D sync)
    setSfPosition(centerPos);
}

void bfCharacter::setRotation(float angle)
{
    //_body->SetTransform(getB2Position(), angle);
}

void bfCharacter::setScale(const sf::Vector2f scale)
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

void bfCharacter::setOrigin(const sf::Vector2f position)
{
	_shape.setOrigin(position);
}

void bfCharacter::setSize(const sf::Vector2f size)
{
    _shape.setSize(size);
}

void bfCharacter::setFillColor(unsigned int color)
{
	setFillColor(sf::Color(color));
}

void bfCharacter::setFillColor(const sf::Color color)
{
	_shape.setFillColor(color);
}

void bfCharacter::setOutlineColor(unsigned int color)
{
	setOutlineColor(sf::Color(color));
}

void bfCharacter::setOutlineColor(const sf::Color color)
{
	_shape.setOutlineColor(color);
}

void bfCharacter::setSpeedBoostTimer(float timer)
{
	_speedBoostTimer = timer;
}

void bfCharacter::setBoostMultiplier(float multiplier)
{
	_boostMultiplier = multiplier;
}

void bfCharacter::updatePosition(float curTime)
{
    bfCharacter::updateAnimation(curTime);

	float multiplier = curTime > _speedBoostTimer ? 1 : _boostMultiplier;

	b2Vec2 vel = _body->GetLinearVelocity();
	vel.Normalize();

	_body->SetLinearVelocity(PLAYER_SPEED * multiplier * vel);
}

void bfCharacter::updateAnimation(float curTime)
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

b2Body* bfCharacter::Body(void) const
{
	return _body;
}

sf::Drawable* bfCharacter::Drawable(void) const
{
	return (sf::Drawable*)&_shape;
}

b2Vec2 bfCharacter::getB2Position(void) const
{
	return _body->GetPosition();
}

sf::Vector2f bfCharacter::getSfPosition(void) const
{
	return _shape.getPosition();
}

WalkStatus bfCharacter::getStatus(void) const
{
	return _status;
}

float bfCharacter::getSpeedBoostTimer(void) const
{
	return _speedBoostTimer;
}

float bfCharacter::getBoostMultiplier(void) const
{
	return _boostMultiplier;
}

WalkDirection bfCharacter::getWalkDirection(b2Vec2 velocity)
{
    if (velocity.x > velocity.y)
    {
        if (velocity.x > 0)
            return WalkDirection::Left;

        return WalkDirection::Right;
    }

    else if (velocity.y > 0)
        return WalkDirection::Down;

    return WalkDirection::Up;
}
