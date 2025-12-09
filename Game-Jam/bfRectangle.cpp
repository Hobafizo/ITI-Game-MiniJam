#include "bfRectangle.h"
#include "def.h"

bfRectangle::bfRectangle(b2Body* body, const sf::Vector2f size, unsigned int color) : _body(body)
{
	setSize(size);
	setOrigin({ size.x / 2, size.y / 2 });
	setFillColor(color);
}

bfRectangle::~bfRectangle(void)
{
}

void bfRectangle::setSfPosition(const sf::Vector2f pos)
{
	_shape.setPosition(pos);
}

void bfRectangle::setPositionFromTopLeft(const sf::Vector2f& topLeft)
{
    sf::Vector2f size = _shape.getSize();

    // Calculate center
    sf::Vector2f centerPos = topLeft + (size * 0.5f);

    // Use your existing position setter (handles Box2D sync)
    setSfPosition(centerPos);
}

void bfRectangle::setRotation(float angle)
{
	_body->SetTransform(getB2Position(), angle);
}

void bfRectangle::setScale(const sf::Vector2f scale)
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

void bfRectangle::setOrigin(const sf::Vector2f position)
{
	_shape.setOrigin(position);
}

void bfRectangle::setSize(const sf::Vector2f size)
{
	_shape.setSize(size);
}

void bfRectangle::setFillColor(unsigned int color)
{
	setFillColor(sf::Color(color));
}

void bfRectangle::setFillColor(const sf::Color color)
{
	_shape.setFillColor(color);
}

void bfRectangle::setOutlineColor(unsigned int color)
{
	_shape.setOutlineColor(sf::Color(color));
}

void bfRectangle::setOutlineColor(const sf::Color color)
{
	_shape.setOutlineColor(color);
}

void bfRectangle::updatePosition(float curTime)
{

}

b2Body* bfRectangle::Body(void) const
{
	return _body;
}

sf::Drawable* bfRectangle::Drawable(void) const
{
	return (sf::Drawable*)&_shape;
}

b2Vec2 bfRectangle::getB2Position(void) const
{
	return _body->GetPosition();
}

sf::Vector2f bfRectangle::getSfPosition(void) const
{
	return _shape.getPosition();
}
void bfRectangle::SetTransparent(bool transparent)
{
	_isTransparent = transparent;
	sf::Color color = _shape.getFillColor();
	color.a = transparent ? 100 : 255; // semi-transparent or opaque
	_shape.setFillColor(color);
}


